
SamplerState globalSampler;

Texture2D<float> shadowMap;

cbuffer PerScene : register(b0)
{
	float3 eyePos;
};

cbuffer PerObject : register(b1)
{
	matrix worldViewProj;
	matrix world;
};

cbuffer Skeleton : register(b2)
{
	matrix bones[128];
};

float4 Tex2D(Texture2D tex, float2 texCoord)
{
	return tex.Sample(globalSampler, texCoord);
}

float3 Tex2D(Texture2D<float3> tex, float2 texCoord)
{
	return tex.Sample(globalSampler, texCoord);
}

float2 Tex2D(Texture2D<float2> tex, float2 texCoord)
{
	return tex.Sample(globalSampler, texCoord);
}

float Tex2D(Texture2D<float> tex, float2 texCoord)
{
	return tex.Sample(globalSampler, texCoord);
}

float4 ClipPosition(float4 position)
{
	return mul(position, worldViewProj);
}

float4 WorldPosition(float4 position)
{
	return mul(position, world);
}

float3 WorldNormal(float3 normal)
{
	return mul(normal, (float3x3)world);
}

void Skin(int4 boneIds, float4 weights, inout float4 position, inout float3 normal)
{
	matrix m = 0;
	
	[unroll]
	for (uint i = 0; i < 4; i++)
	{
		m += bones[boneIds[i]] * weights[i];
	}
		
	position = mul(position, m);
	normal = mul(normal, (float3x3)m);
}

#define SV_GPOSITION SV_TARGET0
#define SV_GNORMAL SV_TARGET1
#define SV_GALBEDOSPEC SV_TARGET2
