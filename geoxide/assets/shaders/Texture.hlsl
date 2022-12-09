
#include "GeoxideShader.hlsl"

Texture2D colorTex;

struct A2V
{
	float4 position : POSITION;
	float2 texCoord : TEXCOORD0;
};

struct V2P
{
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD0;
};

V2P vertex(A2V v)
{
	V2P o;
	
	o.position = CalPos(v.position);
	o.texCoord = v.texCoord;
	
	return o;
}

float4 pixel(V2P p) : SV_TARGET0
{
	return Tex2D(colorTex, p.texCoord);
}


