#include "ContentBrowserPanel.h"
#include "imgui.h"
#include <filesystem>
namespace Engine
{
	const char* s_AssetsDirectory = "assets";
	void ContentBrowserPanel::ImGuiRender()
	{
		ImGui::Begin("Content Browser");

		for (auto& p : std::filesystem::directory_iterator(s_AssetsDirectory))
		{
			std::string path = p.path().string();
			ImGui::Text("%s", path.c_str());
		}

		ImGui::End();
	}
}
