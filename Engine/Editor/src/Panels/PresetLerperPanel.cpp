#include "PresetLerperPanel.h"
#include "imgui.h"
#include "Application/Application.h"
#include <string>
#include "misc/cpp/imgui_stdlib.h"
#include "Engine/Scene/Scene.h"
#include "Engine/Scene/Entity.h"
#include "Engine/Scene/Components.h"
#include <ToolBox/File/FileDialogs.h>
#include <yaml-cpp/yaml.h>
namespace Engine
{
	void PresetLerperPanel::OnImGuiRender(Scene* scene)
	{
		static bool first = false, second = false, third = false;
		static std::string path1, path2;
		static std::string str1 = "";
		ImGui::Begin("PresetLerper");
		{
			ImGui::InputText("First Preset", &str1, ImGuiInputTextFlags_ReadOnly);
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
				{
					const wchar_t* path = (const wchar_t*)payload->Data;
					m_FirstData = Prefab::ReadPrefabData(path);
					path1 = std::filesystem::path(path).string();
					str1 = std::filesystem::path(path).stem().string();
					first = true;
				}
				ImGui::EndDragDropTarget();
			}
		}
		static std::string str2 = "";
		{
			ImGui::InputText("Second Preset", &str2, ImGuiInputTextFlags_ReadOnly);
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
				{
					const wchar_t* path = (const wchar_t*)payload->Data;
					m_SecondData = Prefab::ReadPrefabData(path);
					path2 = std::filesystem::path(path).string();
					str2 = std::filesystem::path(path).stem().string();
					second = true;
				}
				ImGui::EndDragDropTarget();
			}
		}
		static Entity entt;
		static std::string str = "";
		{
			ImGui::InputText("Entity", &str, ImGuiInputTextFlags_ReadOnly);
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ENTITY_ID"))
				{
					entt = *(Entity*)payload->Data;
					str = entt.GetComponent<TagComponent>().tag;
					third = true;
				}
				ImGui::EndDragDropTarget();
			}
		}
		static float lerpValue;
		if (first && second && third)
		{
			ImGui::DragFloat("Blend", &lerpValue, 0.01f, 0, 1);
			Vector3f pos = Vector3f::Lerp(m_FirstData.matrix.GetPosition(), m_SecondData.matrix.GetPosition(), lerpValue);
			Vector3f rot = Vector3f::Lerp(m_FirstData.matrix.GetRotation(), m_SecondData.matrix.GetRotation(), lerpValue);
			Vector3f scale = Vector3f::Lerp(m_FirstData.matrix.GetScale(), m_SecondData.matrix.GetScale(), lerpValue);
			entt.GetComponent<TransformComponent>().transform.SetPosition(pos);
			entt.GetComponent<TransformComponent>().transform.SetRotation(rot);
			entt.GetComponent<TransformComponent>().transform.SetScale(scale);

			
		}
		if (ImGui::Button("Save as.."))
		{
			std::string path = ToolBox::SaveFile("preset lerper (*.PSL)\0*.PSL\0", Application::GetWindow()->GetHwnd());
			if (!path.empty())
			{
				if (!std::filesystem::path(path).has_extension())
				{
					path += ".PSL";
				}
				YAML::Emitter out;
				out << YAML::BeginMap;
				out << YAML::Key << "FirstPath" << YAML::Value << path1;
				out << YAML::Key << "SecondPath" << YAML::Value << path2;
				out << YAML::Key << "EntityId" << YAML::Value << entt.GetId();
				out << YAML::Key << "BlendFactor" << YAML::Value << lerpValue;
				out << YAML::EndMap;
				std::ofstream fout(path);
				fout << out.c_str();
			}
		}
		ImGui::SameLine();
		if (ImGui::Button("load.."))
		{
			std::string path = ToolBox::OpenFile("preset lerper (*.PSL)\0*.PSL\0", Application::GetWindow()->GetHwnd());
			if (!path.empty())
			{
				std::ifstream fin(path);
				std::stringstream buffer;
				buffer << fin.rdbuf();
				uint32_t enttity;
				YAML::Node node = YAML::Load(buffer.str());
				path1 = node["FirstPath"].as < std::string>();
				path2 = node["SecondPath"].as < std::string>();
				enttity = node["EntityId"].as <uint32_t>();
				lerpValue = node["BlendFactor"].as<float>();
				entt = { enttity, scene };
				m_FirstData = Prefab::ReadPrefabData(path1);
				m_SecondData = Prefab::ReadPrefabData(path2);
				str1 = std::filesystem::path(path1).stem().string();
				str2 = std::filesystem::path(path2).stem().string();
				str = entt.GetComponent<TagComponent>().tag;
				first = true;
				second = true;
				third = true;
			}
		}

		static bool FirstFrame = false;
		if (!FirstFrame)
		{
			std::string path = "Assets/Prefab/PresetLerperSettings.PSL";
			std::ifstream fin(path);
			std::stringstream buffer;
			buffer << fin.rdbuf();
			uint32_t enttity;
			YAML::Node node = YAML::Load(buffer.str());
			path1 = node["FirstPath"].as < std::string>();
			path2 = node["SecondPath"].as < std::string>();
			enttity = node["EntityId"].as <uint32_t>();
			lerpValue = node["BlendFactor"].as<float>();
			entt = { enttity, scene };
			m_FirstData = Prefab::ReadPrefabData(path1);
			m_SecondData = Prefab::ReadPrefabData(path2);
			str1 = std::filesystem::path(path1).stem().string();
			str2 = std::filesystem::path(path2).stem().string();
			str = entt.GetComponent<TagComponent>().tag;
			first = true;
			second = true;
			third = true;
			FirstFrame = true;
		}

		ImGui::End();
	}
}