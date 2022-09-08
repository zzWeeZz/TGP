#include "SceneHierarchyPanel.h"
#include "Imgui.h"
#include "Engine/Scene/Components.h"
#include <imgui_internal.h>
#include "misc/cpp/imgui_stdlib.h"
#include "Engine/Scripting/ScriptRegistry.h"
#include "Engine/Assets/AssetPacker.h"
namespace Engine
{
	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& context)
	{
		m_Context = context;
	}

	void SceneHierarchyPanel::SetContext(const Ref<Scene>& context)
	{
		m_Context = context;
		m_SelectedEntity = Entity();
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
			char tagbuffer[512];
			memset(tagbuffer, 0, sizeof(tagbuffer));
			strcpy(tagbuffer, TagComp.tag.c_str());
			if (ImGui::InputText("##", tagbuffer, sizeof(tagbuffer)))
			{
				TagComp.tag = tagbuffer;
			}
			ImGui::SameLine();
			if (ImGui::Button("Add Comp"))
			{
				ImGui::OpenPopup("dasda");

			}
			if (ImGui::BeginPopup("dasda"))
			{

				ImGui::Columns(100 / (100, 100), "akdfjladdddk", true);
				std::vector<std::string> listofComps
				{
					"Model Component",
					"Pointlight Component",
					"SpotLight Component",
					"Script Component",
					"Animator Component"
				};
				for (int n = 0; n < listofComps.size(); n++)
				{
					if ((ImGui::Button(listofComps[n].c_str())))
					{
						if (listofComps[n] == "Model Component")
						{
							entity.AddComponent<ModelComponent>();
						}
						else if (listofComps[n] == "Pointlight Component")
						{
							entity.AddComponent<PointLightComponent>();
						}
						else if (listofComps[n] == "SpotLight Component")
						{
							entity.AddComponent<SpotLightComponent>();
						}
						else if (listofComps[n] == "Script Component")
						{
							entity.AddComponent<ScriptComponent>();
						}
						else if (listofComps[n] == "Animator Component")
						{
							entity.AddComponent<AnimatorComponent>();
						}
					}
					ImGui::NextColumn();
				}


				ImGui::EndPopup();
			}
			const ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap;
		}
		if (entity.HasComponent<TransformComponent>())
		{
			auto& tf = entity.GetComponent<TransformComponent>();
			bool open = ImGui::CollapsingHeader("Transform component");
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
				ImGui::Text("Position: ");
				ImGui::SameLine();
				if (ImGui::DragFloat3("##Pos", imPos, 0.1f))
				{
					tf.transform.SetPosition({ imPos[0], imPos[1], imPos[2] });
				}
				float imRot[3] = { 0 };
				memcpy(imRot, &tf.transform.GetRotation(), sizeof(Vector3f));
				ImGui::Text("Rotation: ");
				ImGui::SameLine();
				if (ImGui::DragFloat3("##Rot", imRot, 0.1f))
				{
					tf.transform.SetRotation({ imRot[0], imRot[1], imRot[2] });
				}
				float imScale[3] = { 0 };
				memcpy(imScale, &tf.transform.GetScale(), sizeof(Vector3f));
				ImGui::Text("Scale: ");
				ImGui::SameLine();
				if (ImGui::DragFloat3("##Sc", imScale, 0.1f))
				{
					tf.transform.SetScale({ imScale[0], imScale[1], imScale[2] });
				}
			}
			ImGui::Separator();

		}
		if (entity.HasComponent<ModelComponent>())
		{
			auto& tf = entity.GetComponent<ModelComponent>();
			bool open = ImGui::CollapsingHeader("Model component");
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

				char buffer[256];
				memset(buffer, 0, sizeof(buffer));
				strcpy(buffer, tf.filePath.c_str());
				ImGui::Text("Filepath: ");
				ImGui::SameLine();
				static std::string strTransfer(buffer);

				if (ImGui::InputText("##sdasdas", buffer, sizeof(buffer)))
				{
					strTransfer = buffer;
					tf.filePath = strTransfer;
				}
				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
					{
						const wchar_t* path = (const wchar_t*)payload->Data;
						strTransfer = std::filesystem::path(path).string().c_str();
						tf.filePath = strTransfer;
						
					}
					ImGui::EndDragDropTarget();
				}
				if (ImGui::Button("Load"))
				{
					tf.modelHandle = Model::Create(tf.filePath.c_str());
				}
			}
			ImGui::Separator();

			if (removeComponent)
			{
				entity.RemoveComponent<ModelComponent>();
			}
		}
		if (entity.HasComponent<AnimatorComponent>())
		{
			auto& tf = entity.GetComponent<AnimatorComponent>();
		
			bool open = ImGui::CollapsingHeader("Animator component");
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
				char tagbuffer[128];
				memset(tagbuffer, 0, sizeof(tagbuffer));
				strcpy(tagbuffer, tf.animatorPath.c_str());
				if (ImGui::InputText("##3", tagbuffer, sizeof(tagbuffer)), ImGuiInputTextFlags_ReadOnly)
				{
					/*tf.animatorPath = tagbuffer;*/
				}
				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
					{
						const wchar_t* path = (const wchar_t*)payload->Data;
						auto newp = std::filesystem::path(path).string().size();
						tf.animatorPath = std::filesystem::path(path).string();
						AssetPacker::ReadAnimator(tf.animatorPath, tf.skPath, tf.specs);
						
					}
					ImGui::EndDragDropTarget();
				}
				if (!tf.specs.empty())
				{
					std::vector<std::string> animNames;
					for (size_t i = 0; i < tf.specs.size(); i++)
					{
						animNames.emplace_back(tf.specs[i].Name);
					}
					static std::string selectedName = "unknown";
					if (ImGui::BeginCombo("Choose animation", selectedName.c_str()))
					{
						for (auto& str : animNames)
						{
							bool isSelected = selectedName == str;
							if (ImGui::Selectable(str.c_str(), isSelected))
							{
								selectedName = str;
							}
							if (isSelected)
							{
								ImGui::SetItemDefaultFocus();
							}
						}
						ImGui::EndCombo();
					}
					if (ImGui::Button("Test run"))
					{
						tf.modelHandle->PlayAnimation(selectedName);
					}
				}
			}
			ImGui::Separator();

			if (removeComponent)
			{
				entity.RemoveComponent<AnimatorComponent>();
			}

		}
			if (entity.HasComponent<SpotLightComponent>())
			{
				auto& pl = entity.GetComponent<SpotLightComponent>();
				bool open = ImGui::CollapsingHeader("Spotlight component");
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
					if (ImGui::DragFloat("CutOff", &pl.cutoff, 0.1f))
					{
					}
				}
				if (removeComponent)
				{
					entity.RemoveComponent<SpotLightComponent>();
				}

			}

		if (entity.HasComponent<PointLightComponent>())
		{
			auto& pl = entity.GetComponent<PointLightComponent>();
			bool open = ImGui::CollapsingHeader("Pointlight component");
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
			}
			if (removeComponent)
			{
				entity.RemoveComponent<PointLightComponent>();
			}

		}
		if (entity.HasComponent<ScriptComponent>())
		{
			auto& sc = entity.GetComponent<ScriptComponent>();
			bool open = ImGui::CollapsingHeader("Script component");
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
				for (auto& script : sc.scripts)
					if (script)
					{
						if (ImGui::CollapsingHeader(script->GetName().c_str()))
						{

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
					}
				auto scriptlist = ScriptRegistry<ScriptBase>::GetNameList();

				if (ImGui::Button("Add Script"))
				{
					ImGui::OpenPopup("adsf");
				}
				ImGui::Separator();

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
			}
			if (removeComponent)
			{
				entity.RemoveComponent<ScriptComponent>();
			}
		}
	}
}
