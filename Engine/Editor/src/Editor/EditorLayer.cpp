#include "EditorLayer.h"
#include "imgui.h"
#include "Engine/Scene/Components.h"
#include "Engine/Renderer/Renderer.h"
#include "ImGuizmo.h"
#include "Engine/Scripting/TestScript.h"
#include "ToolBox/File/FileDialogs.h"
#include "Engine/Scene/SceneSerializer.h"
#include "Application/Application.h"
void Engine::EditorLayer::OnAttach()
{
	myEditorCamera = Camera::Create(90, { 100, 100 }, 0.1, 100000.f);
	myScene = CreateRef<Scene>();
	mySceneHierarchyPanel = CreateRef<SceneHierarchyPanel>(myScene);
	myContentBrowserPanel = CreateRef<ContentBrowserPanel>();
	myThemeEditorPanel = CreateRef<ThemeEditorPanel>();
	myAnimatorEditorPanel = CreateRef<AnimatorEditorPanel>();
	myScene = CreateRef<Scene>();
	mySceneHierarchyPanel->SetContext(myScene);
	SceneSerializer serializer(myScene);
	serializer.Deserialize("Assets/Scenes/TestScene.scn");
	myThemeEditorPanel->StartUp();
}

void Engine::EditorLayer::OnUpdate()
{
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
				auto newp = std::filesystem::path(path).string();
				myScene = CreateRef<Scene>();
				mySceneHierarchyPanel->SetContext(myScene);
				SceneSerializer serializer(myScene);
				serializer.Deserialize(newp);

			}
			ImGui::EndDragDropTarget();
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
			ImGuizmo::Manipulate(&view[0], &proj[0], ops, ImGuizmo::WORLD, &trans[0]);

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
			}
		}
		ImGui::End();

	}
	ImGui::PopStyleVar();

	DrawPanels();

	ImGui::Begin("Hej Lukas!");
	ImGui::Text("Nu ar den Har!");
	ImGui::Text("Theme editor ar dar nere med 'apply' som sparar.");
	ImGui::Text("Scener som sparas och laddas genom file.");
	ImGui::Text("'OBS' Scenen maste ligga i assets/scenes/*name*.scn");
	ImGui::Text("Detta sedan laddar den in Testscene.scn pa motor start!");
	ImGui::Text("MVH");
	ImGui::Text("Niklas Jakobsen");
	ImGui::End();
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
}

void Engine::EditorLayer::DrawPanels()
{
	mySceneHierarchyPanel->OnImGuiRender();
	myContentBrowserPanel->ImGuiRender();
	myThemeEditorPanel->OnImGuiRender();
}
