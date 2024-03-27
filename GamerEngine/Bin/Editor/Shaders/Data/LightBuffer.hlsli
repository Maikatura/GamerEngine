#ifndef LIGHTBUFFER_HLSLI
#define LIGHTBUFFER_HLSLI

struct LightData
{
	
	
	float4x4 LightView[6];			// 16 * 4 Bytes
	float4x4 LightProjection;	// 16 * 4 Bytes
	

		
	float3 Color;
	float Intensity;			// 16 Bytes

	float3 Direction;			
	float Range;				// 16 Bytes

	float3 Position;
	float Attenuation;			// 16 Bytes

	float SpotInnerRadius;
	float SpotOuterRadius;
	uint LightType;
	bool CastShadows;			// 16 Bytes

	float NearPlane;
	float FarPlane;
	uint ShadowMapIndex;
	float VogalDiskScale;

	bool SmoothShadows;
	float3 Padding;

};

#define MAX_LIGHTS 20

cbuffer LightBuffer : register(b3)
{
	LightData LB_DirectionalLight;
	LightData LB_LightsSpot[MAX_LIGHTS];
	LightData LB_LightsPoint[MAX_LIGHTS];
	uint LB_NumLightsSpot;
	uint LB_NumLightsPoint;
	float LB_padding1;
	float LB_padding2;
}

#endif