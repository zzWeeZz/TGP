#include "EditorLayer.h"
#include "imgui.h"
#include "Engine/Scene/Components.h"
#include "Engine/Renderer/Renderer.h"
#include "ImGuizmo.h"
#include "Engine/Scripting/TestScript.h"
#include "ToolBox/File/FileDialogs.h"
#include "Engine/Scene/SceneSerializer.h"
#include "Application/Application.h"
#include "Engine/Scene/Prefab/Prefab.h"
#include "ToolBox/Input/Input.h"
#include "Editor/CommandStack.h"
#include <Engine/Core/Event/ApplicationEvent.h>
void Engine::EditorLayer::OnAttach()
{
	myEditorCamera = Camera::Create(90, { 100, 100 }, 0.1, 100000.f);
	myScene = CreateRef<Scene>();
	mySceneHierarchyPanel = CreateRef<SceneHierarchyPanel>(myScene);
	myContentBrowserPanel = CreateRef<ContentBrowserPanel>();
	myThemeEditorPanel = CreateRef<ThemeEditorPanel>();
	myAnimatorEditorPanel = CreateRef<AnimatorEditorPanel>();
	myPresetLerperPanel = CreateRef<PresetLerperPanel>();
	myScene = CreateRef<Scene>();
	SceneSerializer ser(myScene);
	ser.Deserialize("Assets/Scenes/Sponza.scn");
	mySceneHierarchyPanel->SetContext(myScene);
	
	myThemeEditorPanel->StartUp();
	CommandStack::AddCallback([&](CommandSet set)
		{
			if (set.first == CommandType::Transform)
			{
				Matrix4x4f matData;
				memcpy(&matData, set.second.data(), sizeof(float) * 16);
				uint32_t entity = *(uint32_t*)(set.second.data() + 16);
				Vector3f pos;
				Vector3f rot;
				Vector3f scale;
				matData.Deconstruct(pos, rot, scale);
				Entity entityread = { entity, myScene.get()};
				entityread.GetComponent<TransformComponent>().transform.SetPosition(pos);
				entityread.GetComponent<TransformComponent>().transform.SetRotation(rot);
				entityread.GetComponent<TransformComponent>().transform.SetScale(scale);
			}
		});
}

void Engine::EditorLayer::OnUpdate()
{
	CommandStack::Update();
	static bool pOpen = true;
	static bool opt_fullscreen = true;
	static bool opt_padding = false;
	static bool toolActive_ParticleEditor = false;
	static bool toolActive_AnimatorEditor = false;

	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	if (opt_fullscreen)
	{
		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	}
	else
	{
		dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
	}

	if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
		window_flags |= ImGuiWindowFlags_NoBackground;

	if (!opt_padding)
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockSpacePanel Demo", &pOpen, window_flags);
	if (!opt_padding)
		ImGui::PopStyleVar();

	if (opt_fullscreen)
		ImGui::PopStyleVar(2);

	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{

			if (ImGui::MenuItem("New", "Ctrl+N"))
			{
				myScene = CreateRef<Scene>();
				mySceneHierarchyPanel->SetContext(myScene);
			}

			if (ImGui::MenuItem("Open...", "Ctrl+O"))
			{
				std::string path = ToolBox::OpenFile("Scene (*.scn)\0*.scn\0", Application::GetWindow()->GetHwnd());
				if (!path.empty())
				{
					myScene = CreateRef<Scene>();
					mySceneHierarchyPanel->SetContext(myScene);
					SceneSerializer serializer(myScene);
					serializer.Deserialize(path);
				}
			}

			if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S"))
			{
				std::string path = ToolBox::SaveFile("Scene (*.scn)\0*.scn\0", Application::GetWindow()->GetHwnd());
				if (!path.empty())
				{
					SceneSerializer serializer(myScene);
					serializer.Serialize(path);
				}

			}

			ImGui::Separator();

			if (ImGui::MenuItem("Close", NULL, false))
				pOpen = false;
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Options"))
		{
			if (ImGui::MenuItem("Close", NULL, false))
				pOpen = false;
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Tools"))
		{
			ImGui::MenuItem("Particle Editor", NULL, &toolActive_ParticleEditor);
			ImGui::MenuItem("Animator Editor", NULL, &toolActive_AnimatorEditor);
			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}
	if (toolActive_AnimatorEditor)
	{
		myAnimatorEditorPanel->OnImGuiRender();
	}

	{
		auto framebuff = Renderer::GetMainFramebuffer();
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("ViewPort", &pOpen, ImGuiWindowFlags_NoCollapse | ImGuiDockNodeFlags_AutoHideTabBar);
		if (framebuff->GetSpecs().width != ImGui::GetWindowWidth() || framebuff->GetSpecs().height != ImGui::GetWindowHeight())
		{
			framebuff->Resize({ ImGui::GetWindowWidth(), ImGui::GetWindowHeight() });
			myEditorCamera->SetAspectRatio((float)framebuff->GetSpecs().width, (float)framebuff->GetSpecs().height);
		}
		ImGui::Image((void*)framebuff->GetColorAttachment(0).Get(), { ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y });
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
			{
				const wchar_t* path = (const wchar_t*)payload->Data;
				auto thePath = std::filesystem::path(path);
				if (thePath.extension() == ".scn")
				{
					auto newp = std::filesystem::path(path).string();
					myScene = CreateRef<Scene>();
					mySceneHierarchyPanel->SetContext(myScene);
					SceneSerializer serializer(myScene);
					serializer.Deserialize(newp);
				}
				else if (thePath.extension() == ".pfb")
				{
					Prefab::LoadPrefab(myScene.get(), path);
				}
				else if (thePath.extension() == ".dds")
				{
					Entity entt = mySceneHierarchyPanel->GetSelectedEntity();
					if (entt.HasComponent<ModelComponent>())
					{
						for (auto&  var : entt.GetComponent<ModelComponent>().modelHandle->GetMaterials())
						{
							var->AddDiffuseTexture(Texture2D::Create(thePath));
						}
					}
				}
				else
				{
					std::cout << "ERROR\n";
				}

			}
			ImGui::EndDragDropTarget();
		}

		ImVec2 mouse_pos = ImGui::GetMousePos();
		ImVec2 screen_pos = ImGui::GetWindowPos();
		ImVec2 final_pos;
		final_pos.x = mouse_pos.x - screen_pos.x;
		final_pos.y = mouse_pos.y - screen_pos.y;
		int clickedIentity = -1;
		bool hasSelected = false;
		if (!ImGuizmo::IsUsing())
			if (ToolBox::Input::IsMousePressed(MouseButton::Left) && ImGui::IsWindowFocused())
			{
				clickedIentity = Renderer::GetClickedEntityId(final_pos.x, final_pos.y) - 1;
				hasSelected = true;
			}


		if (clickedIentity > -1 && hasSelected)
		{
			mySceneHierarchyPanel->SetEntity({ (uint32_t)clickedIentity, myScene.get() });
		}


		if (mySceneHierarchyPanel->GetSelectedEntity()() && myScene)
		{
			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();
			float windowWidth = (float)ImGui::GetWindowWidth();
			float windowHeight = (float)ImGui::GetWindowHeight();
			ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);
			float view[16] = { 0 };
			memcpy(view, &myEditorCamera->GetLookAt(), sizeof(float) * 16);
			float proj[16] = { 0 };
			memcpy(proj, &myEditorCamera->GetProjection(), sizeof(float) * 16);
			auto entity = mySceneHierarchyPanel->GetSelectedEntity();
			auto& tf = entity.GetComponent<TransformComponent>();
			auto matrix = tf.transform.GetMatrix();
			float trans[16] = { 0 };
			memcpy(trans, &matrix, sizeof(float) * 16);
			static ImGuizmo::OPERATION ops = ImGuizmo::TRANSLATE;
			static ImGuizmo::MODE mode = ImGuizmo::WORLD;
			if (ImGui::GetIO().KeysDown[ImGuiKey_W])
			{
				ops = ImGuizmo::TRANSLATE;
			}
			if (ImGui::GetIO().KeysDown[ImGuiKey_E])
			{
				ops = ImGuizmo::ROTATE;
			}
			if (ImGui::GetIO().KeysDown[ImGuiKey_R])
			{
				ops = ImGuizmo::SCALE;
			}
			static bool swapper = 0;
			if (ToolBox::Input::IsKeyPressed(Keys::Q))
			{
				swapper = !swapper;
			}
			mode = swapper ? ImGuizmo::WORLD : ImGuizmo::LOCAL;
			ImGuizmo::Manipulate(&view[0], &proj[0], ops, mode, &trans[0]);
			static bool ReadGizmo = false;
			if (ImGuizmo::IsUsing())
			{
				Matrix4x4f matData;
				memcpy(&matData, &trans[0], sizeof(float) * 16);
				
				Vector3f pos;
				Vector3f rot;
				Vector3f scale;
				matData.Deconstruct(pos, rot, scale);

				mySceneHierarchyPanel->GetSelectedEntity().GetComponent<TransformComponent>().transform.SetPosition(pos);
				mySceneHierarchyPanel->GetSelectedEntity().GetComponent<TransformComponent>().transform.SetRotation(rot);
				mySceneHierarchyPanel->GetSelectedEntity().GetComponent<TransformComponent>().transform.SetScale(scale);
				Matrix4x4f matNew = mySceneHierarchyPanel->GetSelectedEntity().GetComponent<TransformComponent>().transform.GetMatrix();
				if (!ReadGizmo)
				{
					CommandStack::Register<CommandType::Transform>(&matData, mySceneHierarchyPanel->GetSelectedEntity().GetId());
					CommandStack::RegisterCurrent<CommandType::Transform>(&matNew, mySceneHierarchyPanel->GetSelectedEntity().GetId());
					ReadGizmo = true;
				}
			}
			else
			{
				ReadGizmo = false;
			}
		}
		ImGui::End();

	}
	ImGui::PopStyleVar();

	DrawPanels();

	/*ImGui::Begin("Hej Lukas!");
	ImGui::Text("Uppgift 2 kom lite sent...");
	ImGui::Text("Men! den som vantar far det dem vill.");
	ImGui::Text("Jag kan ju inte gora nagot simpelt sa...");
	ImGui::Text("Nu finns det prefab system och Entity lerping (ja... entity lerping)");
	ImGui::Text("du kan spara dina settings i PresetLerper™");
	ImGui::Text("Den funkar med drag and drop in engine. saklart!");
	ImGui::Text("MVH");
	ImGui::Text("Niklas Jakobsen");
	ImGui::End();*/
	ImGui::End();
	myEditorCamera->Update(ToolBox::Chrono::Timestep());
	myScene->OnEditorUpdate();
	Renderer::SetActiveCamera(myEditorCamera);
}

void Engine::EditorLayer::OnDetach()
{
}

void Engine::EditorLayer::OnEvent(Event& e)
{
	myContentBrowserPanel->OnEvent(e);
}

void Engine::EditorLayer::DrawPanels()
{
	mySceneHierarchyPanel->OnImGuiRender();
	myContentBrowserPanel->ImGuiRender();
	myThemeEditorPanel->OnImGuiRender();
	//myPresetLerperPanel->OnImGuiRender(myScene.get());
}
