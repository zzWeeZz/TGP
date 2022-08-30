#pragma once
#include "ParticleEmitter.h"
#include <vector>

#include "Engine/Utils/Utilities.h"

namespace Engine
{
	class ParticleSystem
	{
	public:
		ParticleSystem(const std::filesystem::path& filePath);
		void Run();
		void Bind();
		void Draw();
		std::vector<ParticleEmitter>& GetEmitters() { return myEmitters; };
		static Ref<ParticleSystem> Create(const std::filesystem::path& path);
	private:
		std::vector<ParticleEmitter> myEmitters;
	};
}
