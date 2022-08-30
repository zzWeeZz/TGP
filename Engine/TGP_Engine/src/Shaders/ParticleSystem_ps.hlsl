#include "ParticleInfo.hlsli"
#include "ConstBuffers.hlsli"

Texture2D albedo : register(t0);
SamplerState albedoSampler : register(s0);


ParticleOut main(ParticleGeoToPixel input)
{
    ParticleOut output;
    float4 textureColor = albedo.Sample(albedoSampler, input.UV);
    output.color.rgba = textureColor * input.color.rgba;
    return output;
}