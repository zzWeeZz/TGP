#include "Window.h"
#include "ToolBox/Input/Input.h"
#include "Engine/DX11/DX11.h"
#include <imgui_impl_win32.cpp>
#define WIN32_LEAN_AND_MEAN         
bool Engine::Window::InitWindow(const std::wstring& title, int width, int height)
{
	WNDCLASS windowClass = {};
	windowClass.style = CS_VREDRAW | CS_HREDRAW | CS_OWNDC;
	windowClass.lpfnWndProc = Window::WinProc;
	windowClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
	windowClass.lpszClassName = L"TGP";
	RegisterClass(&windowClass);

	myHandle = CreateWindow(
		L"TGP",
		title.c_str(),
		WS_OVERLAPPEDWINDOW | WS_POPUP | WS_VISIBLE,
		0,
		0,
		width,
		height,
		nullptr, nullptr, nullptr,
		this
	);
	FILE* consoleOut;
	AllocConsole();
	freopen_s(&consoleOut, "CONOUT$", "w", stdout);
	setvbuf(stdout, nullptr, _IONBF, 1024);

	return true;
}

LRESULT CALLBACK Engine::Window::WinProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	ToolBox::Input::UpdateEvents(uMsg, wParam, lParam);
	ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);
	if (uMsg == WM_DESTROY || uMsg == WM_CLOSE)
	{
		PostQuitMessage(0);
	}
	else if (uMsg == WM_CREATE)
	{
		const CREATESTRUCT* createdStruct = reinterpret_cast<CREATESTRUCT*>(lParam);
		reinterpret_cast<Window*>(createdStruct->lpCreateParams)->myHandle = hWnd;

		reinterpret_cast<Window*>(createdStruct->lpCreateParams)->myCreateFunc();
	}
	else if (uMsg == WM_SIZE)
	{
		DX11::Resize(LOWORD(lParam), HIWORD(lParam));
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}