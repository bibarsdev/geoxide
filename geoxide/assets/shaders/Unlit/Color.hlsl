
#include "Geoxide.hlsli"

cbuffer Material : register(b0)
{
	float4 surfaceColor;
};

struct A2V
{
	float4 position : POSITION;
};

struct V2F
{
	float4 clip_position : SV_POSITION;
};

V2F vert(A2V v)
{
	V2F o;
	
	o.clip_position = CalcClip(v.position);
	
	return o;
}

float4 frag(V2F p) : SV_TARGET0
{
	float4 o;
	
	o = surfaceColor;
	
	return o;
}

program
{
	pass
	{
		Lighting Forward

		Vertex vert
		Fragment frag
	}
}
