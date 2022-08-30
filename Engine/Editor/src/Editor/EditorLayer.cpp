#include "EditorLayer.h"
#include "imgui.h"
#include "Engine/Scene/Components.h"
#include "Engine/Renderer/Renderer.h"

void Engine::EditorLayer::OnAttach()
{
	myEditorCamera = Camera::Create(90, { 100, 100 }, 0.1, 100000.f);
	myScene = CreateRef<Scene>();
	{
		auto entt = myScene->CreateEntity();
	}
	{
		auto entt = myScene->CreateEntity();
		entt.GetComponent<TagComponent>().tag = "gjdajkkkkkka";
		ModelComponent& mdl = entt.AddComponent<ModelComponent>();
		mdl.filePath = "Model/SM_Particle_Chest.fbx";
	}
	{
		auto entt = myScene->CreateEntity();
	}
	{
		auto entt = myScene->CreateEntity();
	}
	{
		auto entt = myScene->CreateEntity();
	}
	{
		auto entt = myScene->CreateEntity();
	}
	{
		auto entt = myScene->CreateEntity();
	}
	{
		auto entt = myScene->CreateEntity();
	}
	{
		auto entt = myScene->CreateEntity();
	}
	{
		auto entt = myScene->CreateEntity();
	}
	mySceneHierarchyPanel = CreateRef<SceneHierarchyPanel>(myScene);
}

void Engine::EditorLayer::OnUpdate()
{
	static bool pOpen = true;
	static bool opt_fullscreen = true;
	static bool opt_padding = false;
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

	// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
	// because it would be confusing to have two docking targets within each others.
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

	// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpacePanel() will render our background
	// and handle the pass-thru hole, so we ask Begin() to not render a background.
	if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
		window_flags |= ImGuiWindowFlags_NoBackground;

	// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
	// This is because we want to keep our DockSpacePanel() active. If a DockSpacePanel() is inactive,
	// all active windows docked into it will lose their parent and become undocked.
	// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
	// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
	if (!opt_padding)
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockSpacePanel Demo", &pOpen, window_flags);
	if (!opt_padding)
		ImGui::PopStyleVar();

	if (opt_fullscreen)
		ImGui::PopStyleVar(2);

	// Submit the DockSpacePanel
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Options"))
		{
			// Disabling fullscreen would allow the window to be moved to the front of other windows,
			// which we can't undo at the moment without finer window depth/z control.
			ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
			ImGui::MenuItem("Padding", NULL, &opt_padding);
			ImGui::Separator();

			if (ImGui::MenuItem("Flag: NoSplit", "", (dockspace_flags & ImGuiDockNodeFlags_NoSplit) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoSplit; }
			if (ImGui::MenuItem("Flag: NoResize", "", (dockspace_flags & ImGuiDockNodeFlags_NoResize) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoResize; }
			if (ImGui::MenuItem("Flag: NoDockingInCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingInCentralNode) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoDockingInCentralNode; }
			if (ImGui::MenuItem("Flag: AutoHideTabBar", "", (dockspace_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_AutoHideTabBar; }
			if (ImGui::MenuItem("Flag: PassthruCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0, opt_fullscreen)) { dockspace_flags ^= ImGuiDockNodeFlags_PassthruCentralNode; }
			ImGui::Separator();

			if (ImGui::MenuItem("Close", NULL, false))
				pOpen = false;
			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}

	auto framebuff = Renderer::GetMainFramebuffer();
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("ViewPort", &pOpen, ImGuiWindowFlags_NoCollapse);
	if (framebuff->GetSpecs().width != ImGui::GetWindowWidth() || framebuff->GetSpecs().height != ImGui::GetWindowHeight())
	{
		framebuff->Resize({ ImGui::GetWindowWidth(), ImGui::GetWindowHeight() });
		myEditorCamera->SetAspectRatio((float)framebuff->GetSpecs().width, (float)framebuff->GetSpecs().height);
	}
	ImGui::Image((void*)framebuff->GetColorAttachment(0).Get(), { ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y });
	ImGui::End();
	ImGui::PopStyleVar();
	mySceneHierarchyPanel->OnImGuiRender();
	ImGui::Begin("hi3");
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
