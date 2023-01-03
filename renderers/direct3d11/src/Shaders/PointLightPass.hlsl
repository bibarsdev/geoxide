
SamplerState globalSampler;

Texture2D<float4> positionTex;
Texture2D<float3> normalTex;
Texture2D<float4> albedoSpecTex;

Texture2D<float> shadowTex;

struct Light
{
	float3 positon;
	float fallOff;
	float3 color;
};

cbuffer PerScene : register(b0)
{
	float3 eyePosition;
};

cbuffer PerLight : register(b1)
{
	Light light;
	matrix viewProj;
};

struct A2V
{
	float4 position : POSITION;
	float2 texCoord : TEXCOORD;
};

struct V2P
{
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD0;
};

V2P PointLightVertex(A2V v)
{
	V2P o;
	
	o.position = v.position;
	o.texCoord = v.texCoord;
	
	return o;
}

float4 PointLightPixel(V2P p) : SV_TARGET0
{
	float4 position = positionTex.Sample(globalSampler, p.texCoord);
	float3 albedo = albedoSpecTex.Sample(globalSampler, p.texCoord).rgb;
	
	float3 fragColor = albedo * 0.1f;
	
	float4 lightSpacePosition = mul(position, viewProj);
	float3 lightSpaceCoords = lightSpacePosition.xyz / lightSpacePosition.w;
	
	lightSpaceCoords.xy = lightSpaceCoords.xy * 0.5f;
	
	lightSpaceCoords.x = lightSpaceCoords.x + 0.5f;
	lightSpaceCoords.y = 0.5f - lightSpaceCoords.y;
	
	bool2 isSaturated = (saturate(lightSpaceCoords.xy) == lightSpaceCoords.xy);
	
	if (isSaturated.x && isSaturated.y && (lightSpaceCoords.z - 0.001f) > shadowTex.Sample(globalSampler, lightSpaceCoords.xy))
	{

	}
	else
	{
		float specular = albedoSpecTex.Sample(globalSampler, p.texCoord).a;
		float3 worldNormal = normalTex.Sample(globalSampler, p.texCoord);
			
		float3 toEye = normalize(eyePosition - position.rgb);
	
		float3 lightDir = /*normalize(*/-light.positon/* - position.rgb)*/;
		float3 reflectDir = reflect(-lightDir, worldNormal);
		
		fragColor += max(dot(lightDir, worldNormal), 0) * albedo * light.color;
		fragColor += pow(max(dot(toEye, reflectDir), 0), 32) * specular * light.color;
		
	}
	
	return float4(fragColor, 1);
}
