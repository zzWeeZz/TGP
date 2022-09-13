#pragma once
#include "SnowFlake/Snowflake.hpp"
#include <filesystem>
#include "ToolBox/Math/Transform.h"
namespace Engine
{
	struct PrefabData
	{
		ToolBox::Transform matrix;
	};
	class Scene;
	class Entity;
	class Prefab
	{
	public:
		static void SavePrefab(Entity entity, const std::filesystem::path& path);
		static void LoadPrefab(Scene* scene, const std::filesystem::path& path);
		static PrefabData ReadPrefabData(const std::filesystem::path& path);
	};
}