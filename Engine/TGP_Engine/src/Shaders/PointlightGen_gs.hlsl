
struct GSOutput
{
	float4 pos : SV_POSITION;
    uint layer : SV_RenderTargetArrayIndex;
};

cbuffer ShadowMatrices : register(b8)
{
    float4x4 mat[6];
    float3 lightPos;
    float farPlane;
};

[maxvertexcount(18)]
void main(
	triangle float4 input[3] : POSITION, 
	inout TriangleStream<GSOutput> output
)
{
    float4x4 idi = float4x4(1, 0, 0, 0,
                            0, 1, 0, 0,
                            0, 0, 1, 0,
                            0, 0, 0, 1);
	for (uint face = 0; face < 6; face++)
	{
		GSOutput element;
        element.layer = face;
        for (int i = 0; i < 3; ++i)
        {
            
            element.pos = mul(mat[face], input[i]);
            output.Append(element);
        }
        output.RestartStrip();
    }
}