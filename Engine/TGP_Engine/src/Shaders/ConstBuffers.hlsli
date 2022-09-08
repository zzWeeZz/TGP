cbuffer CameraBuffer : register(b0)
{
    float4x4 toView;
    float4x4 toProjection;
    float4 cameraPosition;
};

cbuffer ObjectBuffer : register(b1)
{
    int4 entityId;
    float4x4 toWorld;
    float4x4 BoneData[128];

};

cbuffer DirectionalLightBuffer : register(b2)
{
    float4 colorAndInstensity;
    float3 DirLightDirection;
    float padding;
};

cbuffer PointLightBuffer : register(b3)
{
    struct Data
    {
        float4 colorAndInstensity;
        float3 position;
        float radius;
    } data[2000];
}

cbuffer SpotLightBuffer : register(b4)
{
    struct SpotData
    {
        float4 colorAndIntensity;
        float4 position;
        float4 direction;
        float4 cutOff;
    } spotData[1000];
}