
SamplerState globalSampler;

cbuffer Scene
{
	matrix worldViewProj;
	float4 eyePosition;
};

float4 Tex2D(Texture2D tex, float2 texCoord)
{
	return tex.Sample(globalSampler, texCoord);
}

float4 CalPos(float4 position)
{
	return mul(position, worldViewProj);
}
