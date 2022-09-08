#include "ConstBuffers.hlsli"

struct VertexInput
{
    float4 Position : POSITION;
    float4 Color : COLOR;
    float4 IntancePosition : INSTANCE;
};

struct PixelOutput
{
    float4 Position : SV_Position;
    float4 Color : COLOR;
};

PixelOutput main( VertexInput vinput )
{
    PixelOutput pOut;
    float4 newPos = vinput.Position + vinput.IntancePosition;
    pOut.Position = mul(toProjection, mul(toView, newPos));
    pOut.Color = vinput.Color;
	return pOut;
}