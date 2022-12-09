
#include "Common.hlsl"

cbuffer MaterialProperties
{
	float4 DiffuseColor;
	float4 SpecularColor;
	float4 AmbientColor;
	float SpecularPower;
};

void PhongVertex(float4 in_position, float3 in_normal, out float4 position, out float3 worldNormal, out float3 toEye)
{
	position = mul(in_position, WorldViewProj);
	worldNormal = mul(in_normal, (float3x3) World);
	
	float3 worldPos = mul(in_position.xyz, (float3x3) World);
	
	toEye = EyePosition.xyz - worldPos;
}

void PhongPixel(float3 in_worldNormal, float3 in_toEye, out float4 lambert, out float4 specular)
{
	float3 worldNormal = normalize(in_worldNormal);
	float3 toEye = normalize(in_toEye);
	
	lambert = 0;
	specular = 0;
	
    [unroll]
	for (uint i = 0; i < NumLights; i++)
	{
		Light light = Lights[i];
		
		lambert += Lambert(-light.direction.xyz, worldNormal, light.diffuse, DiffuseColor);
		specular += Specular(toEye, -light.direction.xyz, worldNormal, SpecularPower, light.intensity, SpecularColor, light.diffuse);
	}
	
	lambert = saturate(lambert);
	specular = saturate(lambert);
}
