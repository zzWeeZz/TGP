struct Input
{
    float4 pos : POSITION;
    uint layer : INPUT;
};

cbuffer ShadowMatrices : register(b8)
{
    float4x4 mat[6];
    float3 lightPos;
    float farPlane;
};


struct Output
{
    float4 color : SV_Target;
    float depth : SV_Depth;
};

Output main(Input input)
{
    float lightDistance = length(input.pos.xyz - lightPos);
    
    lightDistance = lightDistance / farPlane;
    Output output;
    output.color = float4(lightDistance, 0, 0, 0);
    output.depth = lightDistance;
    return output;
}