#pragma once
#include "ParticleSystem/ParticleSystem.h"
#include "Engine/Renderer/Mesh/AnimatedMesh.h"
namespace Engine
{
	class AssetPacker
	{
	public:
		static void Particle(Ref<ParticleSystem> ps, const std::string& name);
		static void ReadAnimator(const std::string& path, std::string& skPath, std::vector<AnimationSpecs>& outSpecs);
		static void WriteAnimator(const std::string& path, const std::string& skPath, std::vector<AnimationSpecs>& writeData);
	};
}