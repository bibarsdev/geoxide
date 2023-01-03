
#include "Geoxide.hlsli"

Texture2D mainTex;

cbuffer Material
{
	float specular;
};

struct A2V
{
	float4 position : POSITION;
	float2 texCoord : TEXCOORD0;
};

struct V2P
{
	float4 clip_position : SV_POSITION;
	float2 texCoord : TEXCOORD0;
};

V2P vertex(A2V v)
{
	V2P o;
	
	o.clip_position = CalcClip(v.position);
	
	return o;
}

float4 pixel(V2P p)
{
	float4 o;
	
	o = Tex2D(mainTex, p.texCoord);
	
	return o;
}

technique
{
	pass
	{
		RenderMode Forward
		
		VertexShader vert
		FragmentShader frag
	}
}
