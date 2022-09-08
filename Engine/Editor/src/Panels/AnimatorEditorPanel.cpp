#include "AnimatorEditorPanel.h"
#include "imgui.h"
#include <fstream>
#include <string>
#include <filesystem>
#include "Engine/Renderer/Mesh/AnimatedMesh.h"
#include <ToolBox/File/FileDialogs.h>
#include "Application/Application.h"
#include "yaml-cpp/yaml.h"

void Engine::AnimatorEditorPanel::OnImGuiRender()
{
	static std::string skPath = "";
	ImGui::Begin("Animator Editor");



	if (ImGui::Button("New"))
	{
		m_AnimationSpecs.clear();
		m_SkPath = "";
	}
	ImGui::SameLine();
	if (ImGui::Button("Open..."))
	{
		std::string path = ToolBox::OpenFile("Animator (*.anim)\0*.anim\0", Application::GetWindow()->GetHwnd());
		if (!path.empty())
		{
			m_AnimationSpecs.clear();
			AssetPacker::ReadAnimator(path, m_SkPath, m_AnimationSpecs);
		}
	}
	ImGui::SameLine();
	if (ImGui::Button("Save As..."))
	{
		std::string path = ToolBox::SaveFile("Animator (*.anim)\0*.anim\0", Application::GetWindow()->GetHwnd());
		if (!path.empty())
		{
			AssetPacker::WriteAnimator(path, m_SkPath, m_AnimationSpecs);
		}

	}


	char buffer[256];
	memset(buffer, 0, sizeof(buffer));
	strcpy(buffer, m_SkPath.c_str());
	ImGui::Text("SkPath: ");
	if (ImGui::InputText("##", buffer, sizeof(buffer), ImGuiInputTextFlags_ReadOnly))
	{
		m_SkPath = buffer;
	}
	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
		{
			const wchar_t* path = (const wchar_t*)payload->Data;
			m_SkPath = std::filesystem::path(path).string().c_str();
		}
		ImGui::EndDragDropTarget();
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