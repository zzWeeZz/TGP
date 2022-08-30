#pragma once
#include <string>
#include <Windows.h>
#include <functional>
namespace Engine
{
	class Window
	{
	public:
		bool InitWindow(const std::wstring& title, int width, int height);
		unsigned int GetWidth() { return myWidth; };
		unsigned int GetHeight() { return myHeight; };
		HWND GetHwnd() { return myHandle; };
		float GetAspectRatio() { return (float)myWidth / myHeight; };
		void CreateCallback(const std::function<void()>& callback) { myCreateFunc = callback; };
		static LRESULT CALLBACK WinProc(_In_ HWND hWnd, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam);
	private:
		HWND myHandle{};
		std::function<void()> myCreateFunc;
		unsigned int myWidth;
		unsigned int myHeight;
	};
}
