
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

cbuffer Skeleton : register(b2)
{
	matrix bones[64];
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

float4 TexCube(TextureCube tex, float3 texCoord)
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

void Skin(int4 boneIds, float4 weights, inout float3 position, inout float3 normal)
{
	float3x3 m = 0;
	
	[unroll]
	for (uint i = 0; i < 4; i++)
	{
		if (boneIds[i] != -1)
		{
			m += weights[i] * (float3x3)bones[boneIds[i]];
		}
	}
		
	position = mul(position, m);
	normal = mul(normal, m);
}

#define SV_GPOSITION SV_TARGET0
#define SV_GNORMAL SV_TARGET1
#define SV_GALBEDOSPEC SV_TARGET2
