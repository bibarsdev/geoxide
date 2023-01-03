
#include "Geoxide.hlsli"

cbuffer Material : register(b0)
{
	float3 surfaceColor;
};

struct A2V
{
	float4 position : POSITION;
	float3 normal : NORMAL;
};

struct A2V_Skin
{
	A2V base;
	int4 bones : BONES;
	float4 weights : WEIGHTS;
};

struct V2F
{
	float4 clip_position : SV_POSITION;
	float4 position : POSITION;
	float3 normal : NORMAL;
};

struct P2M
{
	float3 position : SV_GPOSITION;
	float3 normal : SV_GNORMAL;
	float4 albedoSpec : SV_GALBEDOSPEC;
};

V2F vert(A2V v)
{
	V2F o;

	o.clip_position = ClipPosition(v.position);
	o.position = WorldPosition(v.position);
	o.normal = WorldNormal(v.normal);

	return o;
}

V2F vertSkin(A2V_Skin v)
{
	V2F o;

	A2V base = v.base;

	Skin(v.bones, v.weights, base.position, base.normal);

	o = vert(base);

	return o;
}

P2M frag(V2F p)
{
	P2M o;

	o.position = p.position;
	o.normal = normalize(p.normal);
	o.albedoSpec = float4(surfaceColor, 0);

	return o;
}

program
{
	pass
	{
		Lighting Deferred

		Vertex vert
		VertexSkinned vertSkin
		Fragment frag
	}
}
