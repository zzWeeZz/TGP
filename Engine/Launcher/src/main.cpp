#include "Application/Application.h"
#include "Engine/DX11/DX11.h"
#include "Application/Application.h"
#include "EntryPoint.h"
#include "Engine/ImGui/ImGuiLayer.h"
#include "Engine/Core/Layer/LayerStack.h"
#include "Editor/EditorLayer.h"


class SandBox : public Engine::Application
{
public:
	SandBox()
	{
		myLayerStack.Push(new Engine::ImGuiLayer());
		myLayerStack.Push(new Engine::EditorLayer());
	}

};

Engine::Application* Engine::CreateApplication()
{
	return new SandBox();
}