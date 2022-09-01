#pragma once
#include "ToolBox/Math/Vector/Vector.h"
namespace Engine
{
	class ThemeEditorPanel
	{
	public:
		void StartUp();
		void OnImGuiRender();
	private:
		void SaveTheme();
		void LoadTheme();
		Vector3f mySavedColor;
	};
}

