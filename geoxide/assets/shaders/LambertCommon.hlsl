
#include "GeoxideShader.hlsl"

cbuffer MaterialProperties
{
	float4 DiffuseColor;
	float4 AmbientColor;
};

void LambertVertex(float4 in_position, float3 in_normal, out float4 position, out float3 worldNormal)
{
	position = mul(in_position, WorldViewProj);
	worldNormal = mul(in_normal, (float3x3) World);
}

void LambertPixel(float3 in_worldNormal, out float4 lambert)
{
	float3 worldNormal = normalize(in_worldNormal);
	
	lambert = 0;
	
    [unroll]
	for (uint i = 0; i < NumLights; i++)
	{
		Light light = Lights[i];
		
		lambert += Lambert(light.direction.xyz, worldNormal, light.diffuse, DiffuseColor);
	}
	
	lambert = saturate(lambert);
}
