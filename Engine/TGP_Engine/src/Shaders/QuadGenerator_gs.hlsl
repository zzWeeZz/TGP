#include "ParticleInfo.hlsli"
#include "ConstBuffers.hlsli"
[maxvertexcount(4)]
void main(
	point ParticleVertexData input[1], 
	inout TriangleStream<ParticleGeoToPixel> output
)
{
	const float2 offsets[4] = {
        float2(-1.0f, 1.0f),
        float2(1.0f, 1.0f),
        float2(-1.0f, -1.0f),
        float2(1.0f, -1.0f)
    };

    const float2 uvs[4] = {
        float2(0.0f, 0.0f),
        float2(1.0f, 0.0f),
        float2(0.0f, 1.0f),
        float2(1.0f, 1.0f)
    };

    const ParticleVertexData inputParticle = input[0];
    for (unsigned int index = 0; index < 4; ++index)
    {
	    ParticleGeoToPixel outputParticle;
        outputParticle.position = mul(toView, inputParticle.position);
        outputParticle.position.xy += offsets[index] * inputParticle.scale.xy;
        outputParticle.position = mul(toProjection, outputParticle.position);
        outputParticle.color = inputParticle.color;
        outputParticle.lifeTime = inputParticle.lifeTime;
        outputParticle.UV = uvs[index];
        outputParticle.velocity = inputParticle.velocity;
        output.Append(outputParticle);
    }

}