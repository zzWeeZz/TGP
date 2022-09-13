#pragma once
#include "Scene.h"
#include "Entity.h"
#include "Engine/DX11/DX11.h"
namespace YAML
{
	class Emitter;
	class Node;
}
namespace Engine
{
	class SceneSerializer
	{
	public:
		SceneSerializer(Ref<Scene> scene);
		static void SerializeEntity(Engine::Entity entity, YAML::Emitter& out);
		static void DeserializeEntity(Engine::Entity& entity, YAML::Node& node);
		bool Serialize(const std::string& path);
		bool Deserialize(const std::string& path);
	private:
		Ref<Scene> myScene;
	};
}