#include "Model.h"

Engine::Model::Model(const std::filesystem::path& filePath)
{
	myMesh.SetMesh(filePath.string(), m_Materials);
}

void Engine::Model::Draw()
{
	myMesh.SubmitMesh();
}

Engine::Ref<Engine::Model> Engine::Model::Create(const std::filesystem::path& filePath)
{
	return CreateRef<Model>(filePath);
}
