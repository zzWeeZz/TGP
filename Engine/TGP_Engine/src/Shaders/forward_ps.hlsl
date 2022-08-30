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


sampler defaultSampler : register(s0);
Texture2D tex : register(t0);
Texture2D norm : register(t1);
Texture2D materialInfo : register(t2);
TextureCube env : register(t30);

float3 DirectionalLightCalc(float3 normal, float3 albedo)
{
    float3 L = -1 * normalize(DirLightDirection);
    float3 N = normal;
    float LdotN = saturate(dot(L, N));
    float3 C = colorAndInstensity.rgb;
    float Ilight = colorAndInstensity.a;
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
    const float3 specularColor = lerp((float3)0.04, color.xyz, metalness);
    const float3 diffuseColor = lerp((float3)0.0, color.xyz, 1.0f - metalness);

    float3 directColor = EvaluateDirectionalLight(
    diffuseColor,
    specularColor,
    PixelNomal,
    roughness,
    colorAndInstensity.rgb,
    colorAndInstensity.a,
    -DirLightDirection,
    toEye);

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
    float3 final = directColor + ambientLighting;
    final = LinearToGamma(final);
    return float4(final, 1);
}