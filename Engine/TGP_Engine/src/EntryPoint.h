#pragma once

extern Engine::Application* Engine::CreateApplication();

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	uint16_t width = 1280;
	uint16_t height = 720;
	Engine::Application::GetWindow()->CreateCallback([=]() {Engine::DX11::Initialize(width, height, false, true); });
	auto app = Engine::CreateApplication();
	app->Run();
	delete app;
	
	return 0;
}