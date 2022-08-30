struct DefferedOutput
{
	float4 Albedo : SV_TARGET0;
	float4 Normal : SV_TARGET1;
	float4 Material : SV_TARGET2;

	float4 VertexNormal : SV_TARGET3;
	float4 WorldPosition : SV_TARGET4;

	float AmbientOcclusion : SV_TARGET5;
};

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
TextureCube env : register(t3);

DefferedOutput main(PixelInput input)
{
    DefferedOutput result;

    float4 albedoSample = tex.Sample(defaultSampler, input.texcoord0).rgba;
    if (albedoSample.a <= 0.05f)
    {
        discard;
        result.Albedo = 0;
        result.Normal = 0;
        result.Material = 0;
        result.VertexNormal = 0;
        result.WorldPosition = 0;
        result.AmbientOcclusion = 0;
        return result;
    }

    float3 normalMap = norm.Sample(defaultSampler, input.texcoord0).agb;
    float4 material = materialInfo.Sample(defaultSampler, input.texcoord0);

    const float AO = normalMap.b;

    normalMap.z = 0;
    normalMap = 2.0f * normalMap - 1.0f;
    normalMap.z = sqrt(1 - saturate(normalMap.x + normalMap.x + normalMap.y + normalMap.y));

    normalMap = normalize(normalMap);
    float3 PixelNormal = normalize(mul(normalMap, input.tangentBias));

    result.Albedo = albedoSample;
    result.Normal = float4(PixelNormal, 1.0f);
    result.Material = material;
    result.VertexNormal = float4(normalize(input.normal), 1.0f);
    result.WorldPosition = input.worldPosition;
    result.AmbientOcclusion = AO;

    return result;
}