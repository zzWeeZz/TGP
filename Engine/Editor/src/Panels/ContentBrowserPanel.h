#pragma once
#include "Engine/Assets/Texture/Texture2D.h"
#include <Engine/Core/Event/Event.h>
namespace Engine
{
	class ContentBrowserPanel
	{
	public:
		ContentBrowserPanel();
		void ImGuiRender();
		void OnEvent(Event& e);
	private:
		std::filesystem::path myCurrentDir;
		Ref<Texture2D> myTexture;
		Ref<Texture2D> myFile;
	};
}