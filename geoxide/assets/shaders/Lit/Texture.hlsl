
#include "Geoxide.hlsli"

Texture2D<float3> mainTex;

cbuffer Material
{
	float specular;
};

struct A2V
{
	float4 position : POSITION;
	float3 normal : NORMAL;
	float2 texCoord : TEXCOORD0;
};

struct V2P
{
	float4 clip_position : SV_POSITION;
	float2 texCoord : TEXCOORD0;
	float3 position : POSITION;
	float3 normal : NORMAL;
};

struct P2M
{
	float3 position : SV_GPOSITION;
	float3 normal : SV_GNORMAL;
	float4 albedoSpec : SV_GALBEDOSPEC;
};

V2P vertex(A2V v)
{
	V2P o;
	
	o.clip_position = CalcClip(v.position);
	o.position = CalcNDC(v.position.xyz);
	o.normal = CalcNDC(v.normal);
	
	return o;
}

P2M pixel(V2P p)
{
	P2M o;
	
	o.position = p.position;
	o.normal = p.normal;
	o.albedoSpec = float4(Tex2D(mainTex, p.texCoord), specular);
	
	return o;
}

technique
{
	pass
	{
		RenderMode Deferred
		
		VertexShader vert
		FragmentShader frag
	}
}
