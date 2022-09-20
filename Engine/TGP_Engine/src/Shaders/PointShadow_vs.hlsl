#include "ConstBuffers.hlsli"

struct Output
{
    float4 Pos : POSITION;
    uint layer : INPUT;
};

Output main( float4 pos : POSITION )
{
    Output output;
    output.Pos = mul(toWorld, pos);
    return output;
}