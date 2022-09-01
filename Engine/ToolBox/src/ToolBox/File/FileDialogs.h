#pragma once
#include <string>
#include <windows.h>
namespace ToolBox
{
	std::string OpenFile(const char* filter, HWND handle);
	std::string SaveFile(const char* filter, HWND handle);
}