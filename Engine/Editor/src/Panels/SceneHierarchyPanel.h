#pragma once
#include "Engine/DX11/DX11.h"
#include "Engine/Scene/Entity.h"

namespace Engine
{
	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel(const Ref<Scene>& context);

		void SetContext(const Ref<Scene>& context);
		Entity GetSelectedEntity() const { return m_SelectedEntity; }
		void SetEntity(Entity entt) { m_SelectedEntity = entt; }
		void OnImGuiRender();
	private:
		void DrawEntityNode(Entity entity);
		void DrawComponents(Entity entity);

		Ref<Scene> m_Context;
		Entity m_SelectedEntity;
	};
}
