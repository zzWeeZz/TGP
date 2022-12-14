#include "ConstBuffers.hlsli"
#include "Math.hlsli"

struct VertexInput
{
    float4 position : POSITION;
    float4 color0 : COLOR0;
    float4 color1 : COLOR1;
    float4 color2 : COLOR2;
    float4 color3 : COLOR3;
    float2 texcoord0 : UV0;
    float2 texcoord1 : UV1;
    float2 texcoord2 : UV2;
    float2 texcoord3 : UV3;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 biTangent : BITANGENT;
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
    int entiityId : ENTITYID;
};


PixelInput main(VertexInput vInput)
{
	PixelInput pInput;
    float4x4 mvp = mul(toProjection, mul(toView, toWorld));
    pInput.position = mul(mvp, vInput.position);
    pInput.worldPosition = mul(toWorld, vInput.position);
	pInput.color0 = vInput.color0;
	pInput.color1 = vInput.color1;
	pInput.color2 = vInput.color2;
	pInput.color3 = vInput.color3;
    pInput.texcoord0 = vInput.texcoord0;
    pInput.texcoord1 = vInput.texcoord1;
    pInput.texcoord2 = vInput.texcoord2;
    pInput.texcoord3 = vInput.texcoord3;
    pInput.normal = vInput.normal;
    pInput.entiityId = entityId;
    
    float3 newTangent = mul((float3x3) toWorld, vInput.tangent);
    float3 newBiTangent = mul((float3x3) toWorld, vInput.biTangent);
    float3 newNormal = mul((float3x3) toWorld, vInput.normal);


    float3x3 tbn = float3x3(normalize(newTangent), normalize(newBiTangent), normalize(newNormal));
    pInput.tangentBias = tbn;
    
	return pInput;
}