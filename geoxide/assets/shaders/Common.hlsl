
struct Light
{
	float4 direction;
	float4 diffuse;
	float intensity;
};

float4 Lambert(
    float3 lightDir,
    float3 surfaceNormal,
    float4 lightColor,
    float4 surfaceColor
)
{
	float amount = dot(lightDir, surfaceNormal);
	float4 color = amount * lightColor * surfaceColor;
	
	return color;
}

float4 Specular(
    float3 eyeDir,
    float3 lightDir,
    float3 surfaceNormal,
    float power,
    float intensity,
    float4 specularColor,
    float4 lightColor
)
{
	float3 reflectDir = reflect(lightDir, surfaceNormal);
	
	float amount = dot(eyeDir, reflectDir);
	amount = pow(amount, power) * intensity;
	
	float4 specular = amount * specularColor * lightColor;

	return specular;
}

Texture2D DiffuseTexture;
Texture2D<float> SpecularTexture;
SamplerState Sampler;

cbuffer SceneProperties
{
	matrix WorldViewProj;
	matrix World;
	float4 EyePosition;
};

cbuffer SkeletonProperties
{
	matrix bones[64];
};

cbuffer LightProperties
{
	uint NumLights;
	Light Lights[4];
};
