cbuffer CameraBuffer : register(b0)
{
    float4x4 toView;
    float4x4 toProjection;
    float4 cameraPosition;
};

cbuffer ObjectBuffer : register(b1)
{
    float4x4 toWorld;
    float4x4 BoneData[128];

};

cbuffer DirectionalLightBuffer : register(b2)
{
    float4 colorAndInstensity;
    float3 DirLightDirection;
    float padding;
};