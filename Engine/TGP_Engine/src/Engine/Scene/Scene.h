#pragma once
#include "SnowFlake/Snowflake.hpp"

namespace Engine
{
	class Scene
	{
		friend class Entity;
		friend class SceneHierarchyPanel;
		friend class SceneSerializer;
	public:
		Scene() = default;
		void OnEditorStart();
		void OnEditorUpdate();
		void OnEditorRender();

		Entity CreateEntity();
		void DestroyEntity(Entity entity);

	private:
		Snowflake::Registry m_Registry;
	};
}