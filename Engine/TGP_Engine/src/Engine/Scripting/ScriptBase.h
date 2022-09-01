#pragma once
#include "Engine/Scene/Entity.h"
namespace Engine
{
	class ScriptBase
	{
	public:
		ScriptBase(Entity& entity);
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
		virtual void Start() = 0;
		virtual void Update() = 0;
		virtual void OnDestroy() = 0;
	protected:

	private:
		Entity& myEntity;
	};
}