#include "AnimatorEditorPanel.h"
#include "imgui.h"
#include <string>
#include <filesystem>
#include "Engine/Renderer/Mesh/AnimatedMesh.h"
void Engine::AnimatorEditorPanel::OnImGuiRender()
{
	static std::string skPath = "";
	ImGui::Begin("Animator Editor");
	{

		char buffer[256];
		memset(buffer, 0, sizeof(buffer));
		strcpy(buffer, skPath.c_str());
		ImGui::Text("SkPath: ");
		if (ImGui::InputText("##", buffer, sizeof(buffer)))
		{
			skPath = buffer;
		}
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
			{
				const wchar_t* path = (const wchar_t*)payload->Data;
				skPath = std::filesystem::path(path).string().c_str();
			}
			ImGui::EndDragDropTarget();
		}
	}

	ImGui::Separator();
	for (auto& spec : m_AnimationSpecs)
	{
		if (ImGui::CollapsingHeader("new Animation"))
		{
			{
				char buffer[256];
				memset(buffer, 0, sizeof(buffer));
				strcpy(buffer, spec.Name.c_str());
				ImGui::Text("Name: ");
				ImGui::SameLine();
				if (ImGui::InputText("##adsflakdf", buffer, sizeof(buffer)))
				{
					spec.Name = buffer;
				}
			}
			{
				char buffer[256];
				memset(buffer, 0, sizeof(buffer));
				strcpy(buffer, spec.FilePath.string().c_str());
				ImGui::Text("file: ");
				ImGui::SameLine();
				if (ImGui::InputText("##files", buffer, sizeof(buffer), ImGuiInputTextFlags_ReadOnly))
				{
					
				}
				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
					{
						const wchar_t* path = (const wchar_t*)payload->Data;
						spec.FilePath = std::filesystem::path(path).string().c_str();
					}
					ImGui::EndDragDropTarget();
				}
			}

			ImGui::Text("Interpolate");
			ImGui::SameLine();
			ImGui::Checkbox("##Interp", &spec.Interpolate);
			ImGui::Text("Loop");
			ImGui::SameLine();
			ImGui::Checkbox("##Loop", &spec.Loop);
			ImGui::Text("play speed");
			ImGui::SameLine();
			ImGui::InputFloat("##speed", &spec.Speed);

		}
	}


	if (ImGui::Button("Add Animation"))
	{
		AnimationSpecs& spec = m_AnimationSpecs.emplace_back();
		spec.Name = "New Animation";
	}

	ImGui::End();
}
