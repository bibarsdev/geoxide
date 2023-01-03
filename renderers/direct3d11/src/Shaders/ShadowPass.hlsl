
cbuffer PerObject : register(b0)
{
	matrix worldViewProj;
};

struct A2V
{
	float4 position : POSITION;
};

struct V2P
{
	float4 position : SV_POSITION;
};

V2P ShadowVertex(A2V v)
{
	V2P o;
	
	o.position = mul(v.position, worldViewProj);
	
	return o;
}
