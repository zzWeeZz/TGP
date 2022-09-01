#pragma once
#include "Engine/Scene/Entity.h"
#include "ImGuiHelper.h"
namespace Engine
{
	class ScriptBase
	{
		friend class SceneHierarchyPanel;
	public:
		void Create(Entity& entity);
		template<typename T>
		T& AddComponent()
		{
			return myEntity.AddComponent<T>();
		}
		template<typename T>
		T& GetComponent()
		{
			return myEntity.GetComponent<T>();
		}
		template<typename T>
		void RemoveComponent()
		{
			myEntity.RemoveComponent<T>();
		}
		template<typename T>
		bool HasComponent()
		{
			return myEntity.HasComponent<T>();
		}
		virtual Properties SetProperties() = 0;
		virtual void Start() = 0;
		virtual void Update() = 0;
		virtual void OnDestroy() = 0;
	protected:
	private:
		Properties myProperties;
		Entity myEntity;
	};
}