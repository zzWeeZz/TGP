#include "ParticleEmitter.h"

#include "ToolBox/Utility/Chrono.h"
#include "yaml-cpp/yaml.h"
#include "yaml-cpp/node/node.h"
#include "yaml-cpp/node/parse.h"
#include "ToolBox/Utility/UtilityFunctions.hpp"

void Engine::ParticleEmitter::LoadData()
{
	myData.lifeTime = 2.0f;
	myData.startColor = { 0,1,1,1 };
	myData.endColor = { 1,0,0,1 };
	myData.gravityScale = 981;
	myData.startVelocity = { 0, 1200, 0 };
	myData.endVelocity = { 0, 5, 0 };
	myData.spawnRate = 0.1f;
	myData.startSize = 20;

	const size_t maxPart = static_cast<size_t>(ceilf(static_cast<float>(myData.maxParticles)));
	myParticles.resize(maxPart);
	for (size_t i = 0; i < myParticles.size(); ++i)
	{
		InitParticle(i);
	}
	myTexture = Texture2D::Create("PatricleSystems/ParticleStar.dds");
	myVertexBuffer.Initialize(myParticles.data(), static_cast<UINT>(myParticles.size()));
}

void Engine::ParticleEmitter::Update()
{
	static float time = 0;
	for (size_t i = 0; i < myParticles.size(); ++i)
	{
		time += myData.spawnRate * ToolBox::Chrono::Timestep();
		auto& part = myParticles[i];
		part.lifeTime += ToolBox::Chrono::Timestep();
		if((part.lifeTime >= myData.lifeTime) || time > myData.spawnRate)
		{
			InitParticle(i);
			time = 0;
		}
		part.color = Vector4f::Lerp(myData.startColor, myData.endColor, part.lifeTime / myData.lifeTime);
		part.velocity.y -= myData.gravityScale * ToolBox::Chrono::Timestep();
		part.pos += Vector4f(part.velocity.x, part.velocity.y , part.velocity.z, 1) * ToolBox::Chrono::Timestep();
	}
}

void Engine::ParticleEmitter::Bind()
{
	if(!myParticles.empty())
	myVertexBuffer.SetData(myParticles.data(), sizeof(ParticleVertex) * myParticles.size());
	DX11::Context()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	UINT offset = 0;
	DX11::Context()->IASetVertexBuffers(0, 1, myVertexBuffer.GetAddressOf(), myVertexBuffer.StridePtr(), &offset);
	if (myTexture) myTexture->Bind(0, ShaderType::Pixel);
}

void Engine::ParticleEmitter::Draw()
{
	DX11::Context()->Draw(static_cast<UINT>(myParticles.size()), static_cast<UINT>(0));
}

void Engine::ParticleEmitter::InitParticle(size_t particleIndex)
{
	auto& part = myParticles[particleIndex];
	part.lifeTime = 0;
	part.color = myData.startColor;
	part.pos = { -50,0,0,1 };
	part.scale = {myData.startSize, myData.startSize, myData.startSize};
	part.velocity = { ToolBox::randomFloat(myData.startVelocity.x - 100, myData.startVelocity.x + 100), ToolBox::randomFloat(myData.startVelocity.y - 100, myData.startVelocity.y + 100), ToolBox::randomFloat(myData.startVelocity.z - 100, myData.startVelocity.z + 100) };
}
