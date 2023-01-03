
SamplerState globalSampler;

cbuffer PerScene : register(b0)
{
	float3 eyePos;
};

cbuffer PerObject : register(b1)
{
	matrix worldViewProj;
	matrix world;
};

float4 Tex2D(Texture2D tex, float2 texCoord)
{
	return tex.Sample(globalSampler, texCoord);
}

float4 CalcClip(float4 vec)
{
	return mul(vec, worldViewProj);
}

float3 CalcNDC(float3 vec)
{
	return mul(vec, (float3x3)world);
}

#define SV_GPOSITION SV_TARGET0
#define SV_GNORMAL SV_TARGET1
#define SV_GALBEDOSPEC SV_TARGET2
