#include "ConstBuffers.hlsli"



float4 main( float4 pos : POSITION ) : POSITION
{
    return mul(toWorld, pos);
}