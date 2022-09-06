#include "ConstBuffers.hlsli"
#include "PBRFunctions.hlsli"

struct DeferredVertextoPixel
{
    float4 position : SV_POSITION;
    float2 uv : UV;
};

sampler defaultSampler : register(s0);
Texture2D AlbedoTexture : register(t0);
Texture2D NormalTexture : register(t1);
Texture2D MaterialTexture : register(t2);
Texture2D VertexNormalTexture : register(t3);
Texture2D WorldPositionTexture : register(t4);
Texture2D AOTexture : register(t5);
TextureCube env : register(t30);

float4 main(DeferredVertextoPixel input) : SV_TARGET
{
    float4 albedo = AlbedoTexture.Sample(defaultSampler, input.uv);
    if (albedo.a == 0)
    {
        discard;
        return 0;
    }

    float3 normal = NormalTexture.Sample(defaultSampler, input.uv).rgb;
    float4 material = MaterialTexture.Sample(defaultSampler, input.uv);
    float3 vertexNormal = VertexNormalTexture.Sample(defaultSampler, input.uv).rgb;
    float4 worldPosition = WorldPositionTexture.Sample(defaultSampler, input.uv);
    float AO = AOTexture.Sample(defaultSampler, input.uv).r;
    const float3 NewDir = normalize(-DirLightDirection);
    const float metalness = material.r;
    const float roughness = material.g;
    const float emissive = material.b;
    const float emissiveStr = material.a;

    const float3 toEye = normalize(cameraPosition.xyz - worldPosition.xyz);
    const float3 specularColor = lerp((float3) 0.04, albedo.xyz, metalness);
    const float3 diffuseColor = lerp((float3) 0.0, albedo.xyz, 1.0f - metalness);

   

    float3 directColor = EvaluateDirectionalLight(
        diffuseColor,
        specularColor,
        normal,
        roughness,
        colorAndInstensity.rgb,
        colorAndInstensity.a,
        NewDir,
        toEye);

    for (unsigned int i = 0; i < 2000; ++i)
    {
        directColor += EvaluatePointLight(diffuseColor, specularColor, normal, roughness, data[i].colorAndInstensity.xyz, data[i].colorAndInstensity.w, data[i].radius, data[i].position, worldPosition.xyz, toEye);
    }
    
    for (unsigned int i = 0; i < 1000; ++i)
    {
        directColor += EvaluateSpotLight(diffuseColor,
        specularColor,
        normal,
        roughness,
        spotData[i].colorAndIntensity.xyz,
        spotData[i].colorAndIntensity.w,
        spotData[i].cutOff.x,
        spotData[i].position.xyz,
        spotData[i].direction.xyz,
        worldPosition.xyz, toEye);
    }
    
    float3 ambientLighting = EvaluateAmbience(
        env,
        normal,
        vertexNormal,
        toEye,
        roughness,
        AO,
        diffuseColor,
        specularColor,
        defaultSampler
    );
    float3 final = directColor + ambientLighting + albedo * emissive * emissiveStr;
    float3 mapped = float3(1.0, 1, 1) - exp(-final * 5);
    // gamma correction 
    mapped = pow(mapped, (1.0 / 2.2));
   
    return float4(mapped, 1);
}