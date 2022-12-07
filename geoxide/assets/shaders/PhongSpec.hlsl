// specular texture

#include "PhongCommon.hlsl"

struct A2V
{
	float4 position : POSITION; // the 'w' coordinate must be '1'
	float3 normal : NORMAL;
	float2 texCoord : TEXCOORD;
};

struct V2P
{
	float4 position : SV_POSITION;
	float3 worldNormal : NORMAL;
	float2 texCoord : TEXCOORD;
	float3 toEye : POSITION;
};

V2P vertex(A2V v)
{
	V2P o;
	
	PhongVertex(v.position, v.normal, o.position, o.worldNormal, o.toEye);
	
	o.texCoord = v.texCoord;
	
	return o;
}

float4 pixel(V2P p) : SV_TARGET
{
	float4 lambert;
	float4 specular;
	
	PhongPixel(p.worldNormal, p.toEye, lambert, specular);
	
	specular *= SpecularTexture.Sample(Sampler, p.texCoord);
	
	return (AmbientColor + lambert + specular);
}
