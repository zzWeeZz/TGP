#include "Prefab.h"
#include <fstream>
#include "yaml-cpp/yaml.h"
#include "Engine/Scene/Entity.h"
#include "Engine/Scene/SceneSerializer.h"
#include "Engine/Scene/Components.h"
namespace Engine
{
	void Prefab::SavePrefab(Entity entity, const std::filesystem::path& path)
	{
		YAML::Emitter out;
		SceneSerializer::SerializeEntity(entity, out);
		std::ofstream fout(path);
		fout << out.c_str();
	}
	void Prefab::LoadPrefab(Scene* scene, const std::filesystem::path& path)
	{
		std::ifstream fin(path);
		std::stringstream buffer;
		buffer << fin.rdbuf();
		YAML::Node node = YAML::Load(buffer.str());
		Entity ett = scene->CreateEntity();
		SceneSerializer::DeserializeEntity(ett, node);
	}
	PrefabData Prefab::ReadPrefabData(const std::filesystem::path& path)
	{
		Scene* scene = new Scene();
		std::ifstream fin(path);
		std::stringstream buffer;
		buffer << fin.rdbuf();
		YAML::Node node = YAML::Load(buffer.str());
		Entity ett = scene->CreateEntity();
		SceneSerializer::DeserializeEntity(ett, node);
		PrefabData data;
		data.matrix = ett.GetComponent<TransformComponent>().transform;
		delete scene;
		scene = nullptr;
		return data;
	}
}