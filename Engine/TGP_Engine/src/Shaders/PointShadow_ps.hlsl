struct Input
{
    float4 pos : SV_POSITION;
    float4 worldPos : POSITION;
    uint layer : INPUT;
};

cbuffer ShadowMatrices : register(b8)
{
    float4x4 mat[6];
    float4x4 views[6];
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
  
    Output output;
    float dist = distance(input.worldPos.xyz, lightPos.xyz);
    dist /= farPlane;
    output.color = float4(dist, 0, 0, 1);
    output.depth = dist;
    return output;
}