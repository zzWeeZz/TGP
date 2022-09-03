#pragma once
#include "Engine/Assets/Texture/Texture2D.h"
namespace Engine
{
	class ContentBrowserPanel
	{
	public:
		ContentBrowserPanel();
		void ImGuiRender();
	private:
		Ref<Texture2D> myTexture;
		Ref<Texture2D> myFile;
	};
}