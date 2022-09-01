#include "Application.h"
#include "Engine/DX11/DX11.h"
#include "ToolBox/Input/Input.h"
#include "ToolBox/Utility/Chrono.h"
#include "Engine/Renderer/Renderer.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include <ImGuizmo.h>

Engine::Application::Application()
{
	myWindow.InitWindow(L"TGP", 1280, 720);
	Renderer::Initialize();
	myPlayGround.Init();
}

void Engine::Application::Run()
{
	while (myRunApp)
	{
		MSG msg = { 0 };
		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			// Should update input here.

			if (msg.message == WM_QUIT)
			{
				myRunApp = false;
			}
		}
		if (myRunApp)
		{
			ImGui_ImplDX11_NewFrame();
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();
			ImGuizmo::BeginFrame();
			for (const auto& layer : myLayerStack)
			{
				layer->OnUpdate();
			}
			ToolBox::Input::UpdateData();
			ToolBox::Chrono::UpdateTimeData();
			myPlayGround.Update();
			DX11::ClearView();
			myPlayGround.Render();
			
			Renderer::Begin();
			
			ImGui::Render();
		
			ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

			// Update and Render additional Platform Windows
			if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
			{
				ImGui::UpdatePlatformWindows();
				ImGui::RenderPlatformWindowsDefault();
			}
			DX11::Present(true);
		}
	}
	Renderer::Shutdown();
	DX11::CleanUpDX11();
}

void Engine::Application::OnEvent(Event& e)
{
	if (e.GetEventType() == EventType::WindowClose)
	{
		myRunApp = false;
	}
	for (auto it = myLayerStack.end(); it != myLayerStack.begin();)
	{
		(*--it)->OnEvent(e);
		if (e.Handled)
			break;
	}
}


void Engine::Application::PushLayer(Layer* layer)
{
	myLayerStack.Push(layer);
}

void Engine::Application::PushOverlay(Layer* overlay)
{
	myLayerStack.PushOverlay(overlay);
}

