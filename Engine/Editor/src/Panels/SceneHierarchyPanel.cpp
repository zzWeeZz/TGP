#include "SceneHierarchyPanel.h"
#include "Imgui.h"
#include "Engine/Scene/Components.h"
#include <imgui_internal.h>
#include "Engine/Scripting/ScriptRegistry.h"
namespace Engine
{
	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& context)
	{
		m_Context = context;
	}

	void SceneHierarchyPanel::SetContext(const Ref<Scene>& context)
	{
		m_Context = context;
	}

	void SceneHierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin("Hierarchy");

		m_Context->m_Registry.ForEach([&](Snowflake::Entity ent)
			{
				Entity entity = { ent, m_Context.get() };
				DrawEntityNode(entity);

			});

		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
		{
			m_SelectedEntity = {};
		}
		bool destroy = false;
		if (ImGui::BeginPopupContextWindow("one", ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverItems))
		{
			if (ImGui::MenuItem("Create Empty Entity"))
			{
				m_Context->CreateEntity();
			}
			ImGui::EndPopup();
		}

		ImGui::End();

		ImGui::Begin("Properties");
		if (m_SelectedEntity())
		{
			DrawComponents(m_SelectedEntity);
		}
		ImGui::End();
	}

	void SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{
		auto pos = ImGui::GetCursorPos();
		auto& tag = entity.GetComponent<TagComponent>();
		ImGuiTreeNodeFlags flags = ((m_SelectedEntity == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		ImGui::PushID(entity.GetId());

		char buf[32];
		sprintf(buf, "##Object %d", entity.GetId());

		ImGui::SetCursorPos(ImVec2(pos.x, pos.y));
		if (ImGui::Selectable(buf, entity.GetId() == m_SelectedEntity.GetId(), 0, ImVec2(ImGui::GetWindowSize().x, 20))) {
			m_SelectedEntity = entity;
		}
		ImGui::SetItemAllowOverlap();

		ImGui::SetCursorPos(ImVec2(pos.x, pos.y + 2));
		ImGui::Text(tag.tag.c_str());

		ImGui::SetCursorPos(ImVec2(pos.x + 30, pos.y + 5));


		ImGui::SetCursorPos(ImVec2(pos.x, pos.y + 30));

		pos.y += 25;

		ImGui::PopID();

		bool deleted = false;
		if (ImGui::BeginPopupContextWindow(std::to_string(entity.GetId()).c_str(), ImGuiPopupFlags_MouseButtonRight))
		{
			if (ImGui::MenuItem("destroy Entity"))
			{
				deleted = true;
			}
			ImGui::EndPopup();
		}

		if (deleted)
		{
			m_Context->DestroyEntity(m_SelectedEntity);
			if (m_SelectedEntity == entity)
			{
				m_SelectedEntity = {};
			}
		}
	}

	void SceneHierarchyPanel::DrawComponents(Entity entity)
	{
		if (entity.HasComponent<TagComponent>())
		{
			auto& TagComp = entity.GetComponent<TagComponent>();
			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy(buffer, TagComp.tag.c_str());
			ImGui::Text("Tag: ");
			ImGui::SameLine();
			if (ImGui::InputText("##", buffer, sizeof(buffer)))
			{
				TagComp.tag = buffer;
			}
			
		}
		const ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap;

		if (entity.HasComponent<TransformComponent>())
		{
			auto& tf = entity.GetComponent<TransformComponent>();
			bool open = ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), flags, "Transform Component");
			ImGui::SameLine();
			
			if (ImGui::Button("..."))
			{
				ImGui::OpenPopup("ComponentSettings");
			}

			bool removeComponent = false;
			if (ImGui::BeginPopup("ComponentSettings"))
			{
				if (ImGui::MenuItem("Remove Component"))
				{
					removeComponent = true;
				}
				ImGui::EndPopup();
			}
			if (open)
			{
				float imPos[3] = { 0 };
				memcpy(imPos, &tf.transform.GetPosition(), sizeof(Vector3f));
				if (ImGui::DragFloat3("Position", imPos, 0.1f))
				{
					tf.transform.SetPosition({ imPos[0], imPos[1], imPos[2] });
				}
				float imRot[3] = { 0 };
				memcpy(imRot, &tf.transform.GetRotation(), sizeof(Vector3f));
				if (ImGui::DragFloat3("Rotation", imRot, 0.1f))
				{
					tf.transform.SetRotation({ imRot[0], imRot[1], imRot[2] });
				}
				float imScale[3] = { 0 };
				memcpy(imScale, &tf.transform.GetScale(), sizeof(Vector3f));
				if (ImGui::DragFloat3("Scale", imScale, 0.1f))
				{
					tf.transform.SetScale({imScale[0], imScale[1], imScale[2]});
				}
				ImGui::TreePop();
			}
		}
		if (entity.HasComponent<ModelComponent>())
		{
			auto& tf = entity.GetComponent<ModelComponent>();
			bool open = ImGui::TreeNodeEx((void*)typeid(ModelComponent).hash_code(), flags, "Model Component");
			ImGui::SameLine();

			if (ImGui::Button("..."))
			{
				ImGui::OpenPopup("ComponentSettings");
			}

			bool removeComponent = false;
			if (ImGui::BeginPopup("ComponentSettings"))
			{
				if (ImGui::MenuItem("Remove Component"))
				{
					removeComponent = true;
				}
				ImGui::EndPopup();
			}
			if (open)
			{
				char buffer[256];
				memset(buffer, 0, sizeof(buffer));
				strcpy(buffer, tf.filePath);
				ImGui::Text("Filepath: ");
				ImGui::SameLine();
				if (ImGui::InputText("##", buffer, sizeof(buffer)))
				{
					static std::string strTransfer(buffer);
					strTransfer = buffer;
					tf.filePath = strTransfer.c_str();
				}
				if (ImGui::Button("Load"))
				{
					tf.modelHandle = Model::Create(tf.filePath);
				}
				ImGui::TreePop();
			}
			if (removeComponent)
			{
				entity.RemoveComponent<ModelComponent>();
			}
		}
		if (entity.HasComponent<PointLightComponent>())
		{
			auto& pl = entity.GetComponent<PointLightComponent>();
			bool open = ImGui::TreeNodeEx((void*)typeid(PointLightComponent).hash_code(), flags, "Pointlight Component");
			ImGui::SameLine();

			if (ImGui::Button("..."))
			{
				ImGui::OpenPopup("ComponentSettings");
			}

			bool removeComponent = false;
			if (ImGui::BeginPopup("ComponentSettings"))
			{
				if (ImGui::MenuItem("Remove Component"))
				{
					removeComponent = true;
				}
				ImGui::EndPopup();
			}
			if (open)
			{
				float imColor[3] = { 0 };
				memcpy(imColor, &pl.color, sizeof(Vector3f));
				if (ImGui::ColorEdit3("Color", imColor, 0.1f))
				{
					memcpy(&pl.color, imColor, sizeof(Vector3f));
				}
				
				if (ImGui::DragFloat("Intensity", &pl.intensity, 0.1f))
				{
				}
				if (ImGui::DragFloat("Radius", &pl.radius, 0.1f))
				{
				}
				ImGui::TreePop();
			}
			if (removeComponent)
			{
				entity.RemoveComponent<PointLightComponent>();
			}
			
		}
		if (entity.HasComponent<ScriptComponent>())
		{
			auto& sc = entity.GetComponent<ScriptComponent>();
			bool open = ImGui::TreeNodeEx((void*)typeid(ScriptComponent).hash_code(), flags, "Script Component");
			ImGui::SameLine();

			if (ImGui::Button("..."))
			{
				ImGui::OpenPopup("ComponentSettings");
			}

			bool removeComponent = false;
			if (ImGui::BeginPopup("ComponentSettings"))
			{
				if (ImGui::MenuItem("Remove Component"))
				{
					removeComponent = true;
				}
				ImGui::EndPopup();
			}
			if (open)
			{
				ImGui::Separator();
				for (auto& script : sc.scripts)
				if (script)
				{
					ImGui::Text(script->GetName().c_str());
					for (auto& prop : script->myProperties.properties)
					{
						switch (prop.type)
						{
						case PropertyType::Float:
							ImGui::DragFloat(prop.name.c_str(), (float*)prop.varible);
							break;
						case PropertyType::Int:
							ImGui::DragInt(prop.name.c_str(), (int*)prop.varible);
							break;
						case PropertyType::String:
							break;
						case PropertyType::Vector2:
							break;
						case PropertyType::Vector3:
							break;
						case PropertyType::Vector4:
							break;
						default:
							break;
						}
					}
				}
				auto scriptlist = ScriptRegistry<ScriptBase>::GetNameList();

				if (ImGui::Button("Add Script"))
				{
					ImGui::OpenPopup("adsf");
				}

				if (ImGui::BeginPopup("adsf"))
				{
					bool mouseClicked = ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left);
					ImGui::Columns(100 / (100, 100), "akdfjladk", true);
					for (int n = 0; n < scriptlist.size(); n++)
					{
						(ImGui::Button("##", { 100,100 }) && mouseClicked);
						if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
						{
							auto s = ScriptRegistry<ScriptBase>::Create(scriptlist[n].c_str());
							s->Create(entity);
							sc.scripts.push_back(s);
						}
						ImGui::Text(scriptlist[n].c_str());
						ImGui::NextColumn();
					}


					ImGui::EndPopup();
				}

				/*if(ImGui::BeginCombo("add Script", "current"))
				{
					for (int n = 0; n < scriptlist.size(); n++)
					{
						bool is_selected = (currentValue == scriptlist[n]);
						if (ImGui::Selectable(scriptlist[n].c_str(), is_selected))
							currentValue = scriptlist[n];
							if (is_selected)
								ImGui::SetItemDefaultFocus();
					}
					ImGui::EndCombo();
				}*/
				ImGui::TreePop();
			}
			if (removeComponent)
			{
				entity.RemoveComponent<ScriptComponent>();
			}
		}
	}
}