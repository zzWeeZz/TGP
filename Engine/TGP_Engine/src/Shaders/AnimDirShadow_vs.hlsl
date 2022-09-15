#include "ConstBuffers.hlsli"
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
    uint4 BoneIndex : BONEIDS;
    float4 BoneWeight : BONEWEIGHTS;
};
float4 main(VertexInput pos) : SV_POSITION
{
    float4x4 skinnedPosition = 0;
    skinnedPosition += mul(pos.BoneWeight.x, BoneData[pos.BoneIndex.x]);
    skinnedPosition += mul(pos.BoneWeight.y, BoneData[pos.BoneIndex.y]);
    skinnedPosition += mul(pos.BoneWeight.z, BoneData[pos.BoneIndex.z]);
    skinnedPosition += mul(pos.BoneWeight.w, BoneData[pos.BoneIndex.w]);

    float4x4 mvp = mul(toProjection, mul(toView, toWorld));
    float4 vertexWorldPosition = mul(toWorld, mul(pos.position, skinnedPosition));
 
    return mul(mvp, mul(pos.position, skinnedPosition));
}