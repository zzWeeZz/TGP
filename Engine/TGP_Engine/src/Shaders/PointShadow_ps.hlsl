struct Input
{
    float4 pos : SV_POSITION;
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
};

Output main(Input input)
{
  
    Output output;
    output.color = float4(length(input.pos) / farPlane, 0, 0, 1);
    return output;
}