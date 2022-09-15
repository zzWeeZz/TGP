
#include "ConstBuffers.hlsli"
#include "PBRFunctions.hlsli"
#include "math.hlsli"

struct DeferredVertextoPixel
{
    float4 position : SV_POSITION;
    float2 uv : UV;
};

sampler defaultSampler : register(s0);
sampler pointSampler : register(s1);
Texture2D AlbedoTexture : register(t0);
Texture2D NormalTexture : register(t1);
Texture2D MaterialTexture : register(t2);
Texture2D VertexNormalTexture : register(t3);
Texture2D WorldPositionTexture : register(t4);
Texture2D AOTexture : register(t5);

Texture2D ShadowTexture : register(t10);
TextureCube env : register(t30);


float ShadowCalc(float4 pixelPosLightSpace, float3 normal, float3 dir)
{
    float3 projCoords = pixelPosLightSpace.xyz / pixelPosLightSpace.w;
    projCoords.x = projCoords.x * 0.5f + 0.5f;
    projCoords.y = -projCoords.y * 0.5f + 0.5f;
    
    if ((saturate(projCoords.x) == projCoords.x) && (saturate(projCoords.y) == projCoords.y))
    {
        float closetDepth = ShadowTexture.Sample(pointSampler, projCoords.xy).r;
        float currentDepth = projCoords.z;

        float bias = max(0.0005f * (1.0 - dot(normal, dir.xyz)), 0.0005f);
        float shadow = 0;
        uint x = 0;
        uint y = 0;
        ShadowTexture.GetDimensions(x, y);
        float2 texelSize = 1.0f / float2(x, y);
        for (int i = -1; i <= 1; i++)
        {
            for (int j = -1; j <= 1; ++j)
            {
                float pcfDepth = ShadowTexture.Sample(pointSampler, projCoords.xy + float2(i, j) * texelSize).r;
                shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
            }
        }
        shadow /= 9.0f;
        if (projCoords.z > 100.0f)
        {
            shadow = 0.0;
        }
        return shadow;
    }
    return 0;
}

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
   
    float3 directColor = 0;
    for (unsigned int i = 0; i < 1; ++i)
    {
        const float3 newLightDir = normalize(-dirData[i].DirLightDirection.xyz);
        float4 pos = mul(dirData[i].proj, mul((dirData[i].view), worldPosition));
        const float shadow = 1 - ShadowCalc(pos, normal, -newLightDir);
       
        float3 col = EvaluateDirectionalLight(
	    diffuseColor,
	    specularColor,
	    normal,
	    roughness,
	    dirData[i].colorAndInstensity.rgb,
	    dirData[i].colorAndInstensity.a,
		
		-newLightDir, toEye);
            
        directColor = col * shadow;
       

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
    // gamma correction 
    float3 final = pow(directColor + ambientLighting, (1.0 / 2.2));
   
    return float4(final, 1);
}