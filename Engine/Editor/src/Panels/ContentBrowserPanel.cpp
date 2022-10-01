#include "ContentBrowserPanel.h"
#include "imgui.h"
#include <filesystem>
#include <Engine/Core/Event/ApplicationEvent.h>
namespace Engine
{
	const char* s_AssetsDirectory = "assets";
	static std::filesystem::path assetDir = "assets";

	ContentBrowserPanel::ContentBrowserPanel()
	{
		myTexture = Texture2D::Create("Assets/Textures/File.dds");
		myFile = Texture2D::Create("Assets/Textures/Realfile.dds");
		myCurrentDir = assetDir;
	}

	void ContentBrowserPanel::ImGuiRender()
	{
		ImGui::Begin("Content Browser");

		if (myCurrentDir != std::filesystem::path(assetDir))
		{
			if (ImGui::Button("<-"))
			{
				myCurrentDir = myCurrentDir.parent_path();
			}
		}

		bool mouseClicked = ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left);
		float panelwidth = ImGui::GetContentRegionAvail().x;
		ImGui::Columns(panelwidth / (100, 100), 0, false);
		for (auto& directoryEntry : std::filesystem::directory_iterator(myCurrentDir))
		{
			const auto& path = directoryEntry.path();
			std::string filenameString = path.filename().string();

			ImGui::PushID(filenameString.c_str());
			Ref<Texture2D> icon = directoryEntry.is_directory() ? myTexture : myFile;
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
			ImGui::ImageButton((ImTextureID)icon->GetSRV().Get(), { 100, 100 }, { 1, 0 }, { 0, 1 });

			if (ImGui::BeginDragDropSource())
			{
				auto relativePath = std::filesystem::relative(path, assetDir);
				relativePath = assetDir.string()+ "\\" + relativePath.string();
				const wchar_t* itemPath = relativePath.c_str();
				ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t));
				ImGui::EndDragDropSource();
			}

			ImGui::PopStyleColor();
			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				if (directoryEntry.is_directory())
					myCurrentDir /= path.filename();

			}
			ImGui::TextWrapped(filenameString.c_str());

			ImGui::NextColumn();

			ImGui::PopID();
		}

		ImGui::End();
	}
	void ContentBrowserPanel::OnEvent(Event& e)
	{
		if (e.GetEventType() == EventType::AppDrop)
		{
			auto ev = *reinterpret_cast<AppDropEvent*>(&e);
			char dropPath[MAX_PATH];
			char newPath[MAX_PATH];
			memset(dropPath, 0, sizeof(dropPath));
			memset(newPath, 0, sizeof(newPath));
			std::string newCurrPath = myCurrentDir.string() + "\\" + ev.GetPath().filename().string();
			strcpy(dropPath, ev.GetPath().string().c_str());
			strcpy(newPath, newCurrPath.c_str());
			CopyFileA(dropPath, newPath, NULL);
		}
	}
}
