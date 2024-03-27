#ifndef SHADOW_FUNCTIONS_HLSLI
#define SHADOW_FUNCTIONS_HLSLI
#include "Samplers.hlsli"
#include "IndirectFunctions.hlsli"

float SampleShadowAtOffset(Texture2D aShadowMap, float3 aShadowCoordinates, float2 offset, float aTexelSize)
{
	const float lightDepth = aShadowMap.SampleLevel(pointClampSampler, aShadowCoordinates.xy + offset * aTexelSize, 0).r;
	if(lightDepth < aShadowCoordinates.z)
	{
		return 0.0f;
	}
	else
	{
		return 1.0f;
	}
}

//PCF Sampling with 4 samples from:
//https://developer.nvidia.com/gpugems/gpugems/part-ii-lighting-and-shadows/chapter-11-shadow-map-antialiasing
float SampleShadowsPCF4(Texture2D aShadowMap, float3 aShadowCoordinates, float aTexelSize)
{
	float2 offset = (float)(frac(aShadowCoordinates.xy - 0.5f) > 0.25);
	if(offset.y > 1.1)
	{
		offset.y = 0;
	}
	float shadowFactor = SampleShadowAtOffset(aShadowMap, aShadowCoordinates, offset + float2(-1.5, 0.5), aTexelSize)
		+ SampleShadowAtOffset(aShadowMap, aShadowCoordinates, offset + float2(0.5, 0.5), aTexelSize)
		+ SampleShadowAtOffset(aShadowMap, aShadowCoordinates, offset + float2(-1.5, -1.5), aTexelSize)
		+ SampleShadowAtOffset(aShadowMap, aShadowCoordinates, offset + float2(0.5, -1.5), aTexelSize);

	return shadowFactor * 0.25f;
}

//PCF Sampling with 16 samples from:
//https://developer.nvidia.com/gpugems/gpugems/part-ii-lighting-and-shadows/chapter-11-shadow-map-antialiasing
float SampleShadowsPCF16(Texture2D aShadowMap, float3 aShadowCoordinates, float aTexelSize)
{
	float shadowFactor = SampleShadowAtOffset(aShadowMap, aShadowCoordinates, float2(-1.5f, -1.5f), aTexelSize)
		+ SampleShadowAtOffset(aShadowMap, aShadowCoordinates, float2(-0.5f, -1.5f), aTexelSize)
		+ SampleShadowAtOffset(aShadowMap, aShadowCoordinates, float2(0.5f, -1.5f), aTexelSize)
		+ SampleShadowAtOffset(aShadowMap, aShadowCoordinates, float2(1.5f, -1.5f), aTexelSize)
		+ SampleShadowAtOffset(aShadowMap, aShadowCoordinates, float2(-1.5f, -0.5f), aTexelSize)
		+ SampleShadowAtOffset(aShadowMap, aShadowCoordinates, float2(-0.5f, -0.5f), aTexelSize)
		+ SampleShadowAtOffset(aShadowMap, aShadowCoordinates, float2(0.5f, -0.5f), aTexelSize)
		+ SampleShadowAtOffset(aShadowMap, aShadowCoordinates, float2(1.5f, -0.5f), aTexelSize)
		+ SampleShadowAtOffset(aShadowMap, aShadowCoordinates, float2(-1.5f, 0.5f), aTexelSize)
		+ SampleShadowAtOffset(aShadowMap, aShadowCoordinates, float2(-0.5f, 0.5f), aTexelSize)
		+ SampleShadowAtOffset(aShadowMap, aShadowCoordinates, float2(0.5f, 0.5f), aTexelSize)
		+ SampleShadowAtOffset(aShadowMap, aShadowCoordinates, float2(1.5f, 0.5f), aTexelSize)
		+ SampleShadowAtOffset(aShadowMap, aShadowCoordinates, float2(-1.5f, 1.5f), aTexelSize)
		+ SampleShadowAtOffset(aShadowMap, aShadowCoordinates, float2(-0.5f, 1.5f), aTexelSize)
		+ SampleShadowAtOffset(aShadowMap, aShadowCoordinates, float2(0.5f, 1.5f), aTexelSize)
		+ SampleShadowAtOffset(aShadowMap, aShadowCoordinates, float2(1.5f, 1.5f), aTexelSize);

	return shadowFactor * 0.0625f; // 0.0625f = 1/16
}

//SamplerComparisonState ShadowSampler
//{
//   // sampler state
//    Filter = MIN_MAG_MIP_LINEAR;
//    AddressU = Clamp;
//    AddressV = Clamp;

//   // sampler comparison state
//    ComparisonFunc = LESS;
//};


float SampleShadowsVogelDisk(Texture2D aShadowMap, float3 aShadowCoordinates, float aInterleavedGradientNoise, float aVogelScale, int aNumSamples)
{
	float shadowFactor = 0.f;

	// TODO: Add blocker search for early stop

	for(int i = 0; i < aNumSamples; ++i)
	{
		shadowFactor += aShadowMap.SampleCmpLevelZero(pcfLinearSampler, aShadowCoordinates.xy + VogelDiskSample(i, aNumSamples, aInterleavedGradientNoise) * aVogelScale, aShadowCoordinates.z).r;
	}
	shadowFactor /= aNumSamples;

	return shadowFactor;
}

float SampleShadowCubeVogelDisk(TextureCube aShadowMap, float aProjectedDepth, float4 aWorldToLightView, float aRange, float3 aFragToLight, float aInterleavedGradientNoise, float aVogelScale, int aNumSamples)
{
	float shadowFactor = 0.f;

	for(int i = 0; i < aNumSamples; ++i)
	{
		//const float lightDepth = aShadowMap.SampleCmp(ShadowSampler, aShadowCoordinates.xy + VogelDiskSample(i, aNumSamples, aInterleavedGradientNoise) * aVogelScale, 0).r;
		float3 aWorldPosition = aFragToLight + (POISSON_SAMPLES_3D[i] + VogelDiskSampleCube(i, aNumSamples, aInterleavedGradientNoise)) * aVogelScale;
		const float lightDepth = aShadowMap.SampleLevel(linearClampSampler, aWorldPosition, 0).r;
		if(lightDepth >= aProjectedDepth)
		{
			shadowFactor += 1.f;
		}
	}
	shadowFactor /= aNumSamples;

	return shadowFactor;
}

#endif