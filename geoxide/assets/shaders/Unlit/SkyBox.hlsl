
#include "Geoxide.hlsli"

TextureCube mainTex;

struct A2V
{
	float4 position : POSITION;
};

struct V2F
{
	float4 clip_position : SV_POSITION;
	float3 texCoord : TEXCOORD;
};

V2F vert(A2V v)
{
	V2F o;
	
	o.clip_position = CalcClip(v.position);
	o.texCoord = v.position;
	
	return o;
}

float4 frag(V2F p) : SV_TARGET0
{
	float4 o;
	
	o = TexCube(mainTex, p.texCoord);
	
	return o;
}

program
{
	pass
	{
		Lighting Forward
		Cull Front

		Vertex vert
		Fragment frag
	}
}
