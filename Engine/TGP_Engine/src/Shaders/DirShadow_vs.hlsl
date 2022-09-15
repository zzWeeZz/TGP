#include "ConstBuffers.hlsli"

float4 main( float4 pos : POSITION ) : SV_POSITION
{
    float4x4 mvp = mul(toProjection, mul(toView, toWorld));
    return mul(mvp, pos);
}