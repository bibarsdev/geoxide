
struct Light
{
	float4 direction;
	float4 diffuse;
	float intensity;
};

float4 Lambert(
    float3 lightNormal,
    float3 surfaceNormal,
    float4 lightColor,
    float4 surfaceColor
)
{
	float amount = saturate(dot(lightNormal, surfaceNormal));
	float4 color = amount * lightColor * surfaceColor;
	
	return color;
}

float4 Specular(
    float3 toEye,
    float3 lightNormal,
    float3 surfaceNormal,
    float4 specularColor,
    float specularPower,
    float specularIntensity,
    float4 lightColor
)
{
	float amount = saturate(dot(surfaceNormal, normalize(lightNormal + toEye)));
	amount = pow(amount, specularPower) * specularIntensity;
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

cbuffer LightProperties
{
	uint NumLights;
	Light Lights[4];
};
