#pragma once
#include <string>
#include <windows.h>
namespace ToolBox
{
	// Filter example : "Scene (*.scn)\0*.scn\0"
	std::string OpenFile(const char* filter, HWND handle);
	// Filter example : "Scene (*.scn)\0*.scn\0"
	std::string SaveFile(const char* filter, HWND handle);
}