#pragma once
#include "Scene.h"
#include "Engine/DX11/DX11.h"
namespace Engine
{
	class SceneSerializer
	{
	public:
		SceneSerializer(Ref<Scene> scene);

		bool Serialize(const std::string& path);
		bool Deserialize(const std::string& path);
	private:
		Ref<Scene> myScene;
	};
}