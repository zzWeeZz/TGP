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
    struct DirData
    {
        float4 colorAndInstensity;
        float4 DirLightDirection;
        float4x4 view;
        float4x4 proj;
    } dirData[8];
   
};

cbuffer PointLightBuffer : register(b3)
{
    struct Data
    {
        float4 colorAndInstensity;
        float3 position;
        float radius;
        float4x4 transforms[6];
    } data[128];
}

cbuffer SpotLightBuffer : register(b4)
{
    struct SpotData
    {
        float4 colorAndIntensity;
        float4 position;
        float4 direction;
        float4 spotInfo;
    } spotData[1000];
}