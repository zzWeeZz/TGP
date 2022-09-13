
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


float exposureSettings(float aperture, float shutterSpeed, float sensitivity)
{
    return log2((aperture * aperture) / shutterSpeed * 100.0 / sensitivity);
}

float exposure(float ev100)
{
    return 1.0 / (pow(2.0, ev100) * 1.2);
}

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
    const float metalness = material.r;
    const float roughness = material.g;
    const float emissive = material.b;
    const float emissiveStr = material.a;

    const float3 toEye = normalize(cameraPosition.xyz - worldPosition.xyz);
    const float3 specularColor = lerp((float3) 0.04, albedo.xyz, metalness);
    const float3 diffuseColor = lerp((float3) 0.0, albedo.xyz, 1.0f - metalness);

   
    float3 directColor;
    for (unsigned int i = 0; i < 8; ++i)
    {
        const float3 newLightDir = normalize(-dirData[i].DirLightDirection.xyz);
        directColor += EvaluateDirectionalLight(
	    diffuseColor,
	    specularColor,
	    normal,
	    roughness,
	    dirData[i].colorAndInstensity.rgb,
	    dirData[i].colorAndInstensity.a,
		
		-newLightDir, toEye);
    }

    for (unsigned int i = 0; i < 32; ++i)
    {
        directColor += EvaluatePointLight(diffuseColor, specularColor, normal, roughness, data[i].colorAndInstensity.xyz, data[i].colorAndInstensity.w, data[i].radius, data[i].position, worldPosition.xyz, toEye);
    }
    
    for (unsigned int i = 0; i < 16; ++i)
    {
        directColor += EvaluateSpotLight(diffuseColor,
        specularColor,
        normal,
        roughness,
        spotData[i].colorAndIntensity.xyz,
        spotData[i].colorAndIntensity.w,
        spotData[i].spotInfo.x,
        spotData[i].position.xyz,
        spotData[i].direction.xyz,
        spotData[i].spotInfo.y, spotData[i].spotInfo.z,
        toEye, worldPosition.xyz);
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
    float ev100 = exposureSettings(10, 10, 10);
    float4 emissiveCol = albedo * emissive * emissiveStr;
    emissiveCol.xyz = emissiveCol.rgb * pow(2.0, ev100 + emissiveCol.w - 3.0);
    float3 final = directColor + ambientLighting + emissiveCol.xyz;
    float3 mapped = float3(1.0, 1, 1) - exp(-final * 5);
    // gamma correction 
    mapped = pow(mapped, (1.0 / 2.2));
   
    return float4(mapped, 1);
}