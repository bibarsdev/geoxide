
#include "GeoxideShader.hlsl"

cbuffer Material
{
	float4 surfaceColor;
};

struct A2V
{
	float4 position : POSITION;
};

struct V2P
{
	float4 position : SV_POSITION;
};

V2P vertex(A2V v)
{
	V2P o;
	
	o.position = CalPos(v.position);
	
	return o;
}

float4 pixel(V2P p) : SV_TARGET0
{
	return surfaceColor;
}


