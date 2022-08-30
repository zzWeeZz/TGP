struct ParticleVertexData
{
	float4 position : POSITION;
    float4 color : COLOR;
    float3 velocity : VELOCITY;
    float3 scale : SCALE;
    float lifeTime : LIFETIME;
};

struct ParticleGeoToPixel
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    float3 velocity : VELOCITY;
    float lifeTime : LIFETIME;
    float2 UV : UV;
};

struct ParticleOut
{
    float4 color : SV_TARGET;
};