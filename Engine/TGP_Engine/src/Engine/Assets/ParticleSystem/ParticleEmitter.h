#pragma once
#include "Engine/Assets/Texture/Texture2D.h"
#include "Engine/Utils/Utilities.h"
#include "ToolBox/Math/Vector/Vector.h"
#include "Engine/Renderer/Buffers/VertexBuffer.h"
#include "Engine/Renderer/Mesh/ModelData.hpp"
#include "yaml-cpp/node/node.h"

namespace Engine
{
	struct ParticleEmitterData
	{
		float spawnRate = 1.0f;
		float spawnRateVariance = 0.0f;
		float lifeTime;
		float lifeTimeVariance;
		Vector3f startVelocity;
		Vector3f endVelocity;
		float gravityScale;
		float startSize;
		float endSize;
		Vector4f startColor;
		Vector4f endColor;
		uint32_t maxParticles = 100;
	};

	class ParticleEmitter
	{
		friend class ParticleSystem;
	public:
		ParticleEmitter() = default;
		void LoadData();
		void Play();
		void Update();
		void Bind();
		void Draw();
		void Stop();
		void InitParticle(size_t particleIndex);

	private:		
		ParticleEmitterData myData;
		Ref<Texture2D> myTexture;
		VertexBuffer<ParticleVertex> myVertexBuffer;
		std::vector<ParticleVertex> myParticles;
	};
}

