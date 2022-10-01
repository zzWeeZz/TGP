#include "ConstBuffers.hlsli"
#include "PBRFunctions.hlsli"

struct PixelInput
{
    float4 position : SV_POSITION;
    float4 worldPosition : POSITION;
    float4 color0 : COLOR0;
    float4 color1 : COLOR1;
    float4 color2 : COLOR2;
    float4 color3 : COLOR3;
    float2 texcoord0 : UV0;
    float2 texcoord1 : UV1;
    float2 texcoord2 : UV2;
    float2 texcoord3 : UV3;
    float3 normal : NORMAL;
    float3x3 tangentBias : TBASIS;
};

sampler pointSampler : register(s1);
sampler defaultSampler : register(s0);
Texture2D tex : register(t0);
Texture2D norm : register(t1);
Texture2D ShadowTexture : register(t10);
Texture2D materialInfo : register(t2);
TextureCube env : register(t30);

float3 DirectionalLightCalc(float3 normal, float3 albedo, float4 colorAndIntensity, float3 dir)
{
    float3 L = -1 * normalize(dir);
    float3 N = normal;
    float LdotN = saturate(dot(L, N));
    float3 C = colorAndIntensity.rgb;
    float Ilight = colorAndIntensity.a;
    float3 Ipixel = LdotN * C * Ilight;

    float3 diffuse = albedo * Ipixel;
    return diffuse;
}

float3 AmbientLightCalc(float3 normal, float3 albedo)
{
    float3 environmentColor = env.SampleLevel(defaultSampler, normal, 5).rgb;
    float3 ambient = albedo * environmentColor;
    return ambient;
}
float exposureSettings(float aperture, float shutterSpeed, float sensitivity)
{
    return log2((aperture * aperture) / shutterSpeed * 100.0 / sensitivity);
}
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
float exposure(float ev100)
{
    return 1.0 / (pow(2.0, ev100) * 1.2);
}
float4 main(PixelInput pInput) : SV_TARGET
{
    float3 color = tex.Sample(defaultSampler, pInput.texcoord0.xy).xyz;
    float3 nomal = norm.Sample(defaultSampler, pInput.texcoord0.xy).agb;
    float4 matInfo = materialInfo.Sample(defaultSampler, pInput.texcoord0.xy);

    const float AO = nomal.b;
    const float metalness = matInfo.r;
    const float roughness = matInfo.g;
    const float emissive = matInfo.b;
    nomal.z = 0;
    nomal = 2.0f * nomal - 1.0f;
    nomal.z = sqrt(1 - saturate(nomal.x + nomal.x + nomal.y + nomal.y));

    nomal = normalize(nomal);
    float3 PixelNomal = normalize(mul(nomal, pInput.tangentBias));

    const float3 toEye = normalize(cameraPosition.xyz - pInput.worldPosition.xyz);
    const float3 specularColor = lerp((float3) 0.04, color.xyz, metalness);
    const float3 diffuseColor = lerp((float3) 0.0, color.xyz, 1.0f - metalness);

    float3 directColor;
    for (unsigned int i = 0; i < 8; ++i)
    {
        const float3 newLightDir = normalize(-dirData[i].DirLightDirection.xyz);
        float4 pos = mul(dirData[i].proj, mul((dirData[i].view), pInput.worldPosition));
        const float shadow = 1 - ShadowCalc(pos, PixelNomal, -newLightDir);
        directColor += EvaluateDirectionalLight(
	    diffuseColor,
	    specularColor,
	    PixelNomal,
	    roughness,
	    dirData[i].colorAndInstensity.rgb,
    	dirData[i].colorAndInstensity.a,
		toEye,
		-newLightDir) * shadow;
    }
		

    for (unsigned int i = 0; i < 32; ++i)
    {
        
        directColor += EvaluatePointLight(diffuseColor, specularColor, PixelNomal, roughness, data[i].colorAndInstensity.xyz, data[i].colorAndInstensity.w, data[i].radius, data[i].position, pInput.worldPosition.xyz, toEye);
    }
    
    for (unsigned int i = 0; i < 16; ++i)
    {
        directColor += EvaluateSpotLight(diffuseColor,
        specularColor,
        PixelNomal,
        roughness,
        spotData[i].colorAndIntensity.xyz,
        spotData[i].colorAndIntensity.w,
        spotData[i].spotInfo.x,
        spotData[i].position.xyz,
        spotData[i].direction.xyz,
        spotData[i].spotInfo.y, spotData[i].spotInfo.z,
        toEye, pInput.worldPosition.xyz);
    }
    
    float3 ambientLighting = EvaluateAmbience(
	env,
	PixelNomal,
	pInput.normal,
	toEye,
	roughness,
	AO,
	diffuseColor,
	specularColor,
	defaultSampler
);
    float ev100 = exposureSettings(10, 10, 10);
    float3 emissiveCol = color * emissive * matInfo.a;

    float3 final = pow(directColor + ambientLighting + emissiveCol.xyz, (1.0 / 2.2));
    return float4(final, 1);
}