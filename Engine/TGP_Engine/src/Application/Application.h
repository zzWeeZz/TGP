#pragma once
#include <d3d11.h>
#include "Window/Window.h"
#include "PlayGround.h"
#include "Engine/Core/Layer/LayerStack.h"
#include "Engine/ImGui/ImGuiLayer.h"
namespace Engine
{
	class Application
	{
	public:
		Application();
		// creates new instance of the window
		void Run();
		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		inline static Window* GetWindow() { return &myWindow; };
	protected:
		PlayGround myPlayGround;
		LayerStack myLayerStack;
		bool myRunApp = true;
		inline static Window myWindow;
	};
	Application* CreateApplication();
}
