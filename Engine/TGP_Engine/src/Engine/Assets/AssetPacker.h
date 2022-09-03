#pragma once
#include "ParticleSystem/ParticleSystem.h"
namespace Engine
{
	class AssetPacker
	{
	public:
		static void Particle(Ref<ParticleSystem> ps, const std::string& name);
	};
}