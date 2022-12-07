
#include "LambertCommon.hlsl"

struct A2V
{
	float4 position : POSITION; // the 'w' coordinate must be '1'
	float3 normal : NORMAL;
};

struct V2P
{
	float4 position : SV_POSITION;
	float3 worldNormal : NORMAL;
};

V2P vertex(A2V v)
{
	V2P o;
	
	LambertVertex(v.position, v.normal, o.position, o.worldNormal);
	
	return o;
}

float4 pixel(V2P p) : SV_TARGET
{
	float4 lambert;
	
	LambertPixel(p.worldNormal, lambert);
	
	return (AmbientColor + lambert);
}
