#include "AnimatedModel.h"
#include "Engine/Utils/AssetHandler.h"

Engine::AnimatedModel::AnimatedModel(const std::filesystem::path& path)
{
	myMesh.SetMesh(path, m_Materials);
}

Engine::AnimatedModel::AnimatedModel(AnimatedModel& other)
{
	myMesh = other.myMesh;
	myAnimationRegistry = other.myAnimationRegistry;
}

Engine::Ref<Engine::AnimatedModel> Engine::AnimatedModel::Create(const std::filesystem::path& path)
{
	return std::make_shared<AnimatedModel>(path);
}
