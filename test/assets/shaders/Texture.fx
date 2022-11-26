
struct A2V
{
	float4 position : POSITION; // the 'w' coordinate must be '1'
	float2 texCoord : TEXCOORD;
};

struct V2P
{
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD;
};

matrix gWorldViewProj;

V2P vertex(A2V v)
{
	V2P o;
	
	o.position = mul(o.position, gWorldViewProj);
	o.texCoord = v.texCoord;
	
	return o;
}

Texture2D gDiffuseTex;
SamplerState gSampler;

float4 pixel(V2P p) : SV_TARGET0
{
	float4 o = gDiffuseTex.Sample(gSampler, p.texCoord);
	return o;
}
