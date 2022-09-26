
struct GSOutput
{
    float4 pos : SV_POSITION;
    uint layer : SV_RenderTargetArrayIndex;
};

cbuffer ShadowMatrices : register(b8)
{
    float4x4 mat[6];
    float4x4 views[6];
    float3 lightPos;
    float farPlane;
};

[maxvertexcount(18)]
void main(
	triangle float4 input[3] : POSITION,
	inout TriangleStream<GSOutput> output
)
{
    for (uint face = 0; face < 6; face++)
    {
        GSOutput element;
        element.layer = face;
        for (int i = 0; i < 3; ++i)
        {
            element.pos = mul(mat[face], input[i]);
            float3 viewPos = mul(views[face]
            , input[i]).
            xyz;
            element.pos.z = length(viewPos) * element.pos.w / farPlane;
            output.Append(element);
        }
        output.RestartStrip();
    }
}