
struct A2V
{
	float4 position : POSITION; // the 'w' coordinate must be '1'
	float4 color : COLOR;
};

struct V2P
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

matrix gWorldViewProj;

V2P vertex(A2V v)
{
	V2P o;
	
	o.position = mul(v.position, gWorldViewProj);
	o.color = v.color;
	
	return o;
}

float4 pixel(V2P p) : SV_TARGET0
{
	float4 o = p.color;
	return o;
}
