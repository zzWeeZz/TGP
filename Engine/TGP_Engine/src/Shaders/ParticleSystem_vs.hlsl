#include "ParticleInfo.hlsli"
#include "ConstBuffers.hlsli"

ParticleVertexData main( ParticleVertexData input )
{
	ParticleVertexData output;
	const float4 particleLocal = input.position;
	const float4 particleGlobal = mul( toWorld, particleLocal );
	output.position = particleGlobal;
    output.color = input.color;
    output.lifeTime = input.lifeTime;
    output.scale = input.scale;
	output.velocity = input.velocity;
    return output;
}