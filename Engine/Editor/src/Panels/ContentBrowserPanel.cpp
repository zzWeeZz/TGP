#include "ContentBrowserPanel.h"
#include "imgui.h"
#include <filesystem>
namespace Engine
{
	const char* s_AssetsDirectory = "assets";
	ContentBrowserPanel::ContentBrowserPanel()
	{
		myTexture = Texture2D::Create("Textures/File.dds");
	}
	void ContentBrowserPanel::ImGuiRender()
	{
		ImGui::Begin("Content Browser");
		static std::string assetDir = "assets";
		bool mouseClicked = ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left);
		float panelwidth = ImGui::GetContentRegionAvail().x;
		ImGui::Columns(panelwidth / (100, 100), 0, false);
		for (auto& p : std::filesystem::directory_iterator(assetDir))
		{
			std::string path = p.path().string();
			(ImGui::ImageButton((void*)myTexture->GetSRV().Get(), {100,100}) && mouseClicked);
			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				assetDir = path;
				break;
			}
			ImGui::Text(path.c_str());
			ImGui::NextColumn();
		}

		ImGui::End();
	}
}
