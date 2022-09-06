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
			ReadFromFile(path);
		}
	}
	ImGui::SameLine();
	if (ImGui::Button("Save As..."))
	{
		std::string path = ToolBox::SaveFile("Animator (*.anim)\0*.anim\0", Application::GetWindow()->GetHwnd());
		if (!path.empty())
		{
			WriteToFile(path);
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




void Engine::AnimatorEditorPanel::WriteToFile(const std::string& path)
{
	YAML::Emitter out;
	out << YAML::BeginMap;
	out << YAML::Key << "SkPath" << YAML::Value << m_SkPath;
	out << YAML::Key << "AnimSpecs" << YAML::Value << YAML::BeginSeq;
	for (auto& spec : m_AnimationSpecs)
	{
		out << YAML::BeginMap;
		out << YAML::Key << "Name" << YAML::Value << spec.Name;
		out << YAML::Key << "skPath" << YAML::Value << spec.FilePath.string();
		out << YAML::Key << "Interp" << YAML::Value << spec.Interpolate;
		out << YAML::Key << "Loop" << YAML::Value << spec.Loop;
		out << YAML::Key << "Speed" << YAML::Value << spec.Speed;
		out << YAML::EndMap;
	}
	out << YAML::EndSeq;
	out << YAML::EndMap;
	std::ofstream fout(path);
	fout << out.c_str();
}

void Engine::AnimatorEditorPanel::ReadFromFile(const std::string& path)
{
	std::ifstream fin(path);
	std::stringstream buffer;
	buffer << fin.rdbuf();
	YAML::Node node = YAML::Load(buffer.str());
	m_SkPath = node["SkPath"].as<std::string>();
	std::vector<YAML::Node> specs = node["AnimSpecs"].as<std::vector<YAML::Node>>();
	for (auto& spec : specs)
	{
		AnimationSpecs& s = m_AnimationSpecs.emplace_back();
		s.Name = spec["Name"].as<std::string>();
		s.FilePath = spec["skPath"].as<std::string>();
		s.Interpolate = spec["Interp"].as<bool>();
		s.Loop = spec["Loop"].as<bool>();
		s.Speed = spec["Speed"].as<float>();
	}
}
