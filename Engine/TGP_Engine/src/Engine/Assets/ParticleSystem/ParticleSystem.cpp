#include "ParticleSystem.h"

Engine::ParticleSystem::ParticleSystem(const std::filesystem::path& filePath)
{
	ParticleEmitter emitter;
	emitter.LoadData();
	myEmitters.push_back(emitter);
}

void Engine::ParticleSystem::Run()
{
	for (auto& particle : myEmitters)
	{
		particle.Update();
	}
}

void Engine::ParticleSystem::Bind()
{
	for ( auto& particle : myEmitters )
	{
		particle.Bind();
	}
}

void Engine::ParticleSystem::Draw()
{
	for (auto& particle : myEmitters)
	{
		particle.Draw();
	}
}

Engine::Ref<Engine::ParticleSystem> Engine::ParticleSystem::Create(const std::filesystem::path& path)
{
	return CreateRef<ParticleSystem>(path);
}
