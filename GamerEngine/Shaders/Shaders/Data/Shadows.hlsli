#ifndef SHADOWS_HLSLI
#define SHADOWS_HLSLI

#include "LightBuffer.hlsli"
#include "Samplers.hlsli"
#include "ShaderStructs.hlsli"
#include "ShadowFunctions.hlsli"
#include "Textures.hlsli"


bool GetShadowPixel(Texture2D aShadowMap, float4x4 aLightView, float4x4 aLightProjection, float3 aWorldPosition, float aBias, bool aCastShadows)
{
	if (!aCastShadows) return false;
	float4 w2lView = mul(aLightView, float4(aWorldPosition, 1));
	float4 v2lProj = mul(aLightProjection, w2lView);

	float2 lightUV;
	lightUV.x = v2lProj.x / v2lProj.w / 2.f + .5f;
	lightUV.y = -v2lProj.y / v2lProj.w / 2.f + .5f;

	if (saturate(lightUV.x) == lightUV.x && saturate(lightUV.y) == lightUV.y)
	{
		float vDepth = (v2lProj.z / v2lProj.w) - aBias;
		float lDepth = aShadowMap.Sample(pointClampSampler, lightUV).r;
		return (lDepth < vDepth);
	}
	return false;
}

bool GetShadowPixel(TextureCube aShadowMap, float4x4 aLightView[6], float4x4 aLightProjection, float aRange, float3 aLightPosition, float3 aWorldPosition, float aBias, bool aCastShadows)
{
	if (!aCastShadows) return false;

	for (int face = 0; face < 6; face++)
	{
		float4 w2lView = mul(aLightView[face], float4(aWorldPosition, 1));
		float4 v2lProj = mul(aLightProjection, w2lView);

		float3 fragToLight = aWorldPosition - aLightPosition;

		float2 lightUV;
		lightUV.x = v2lProj.x / v2lProj.w / 2.f + .5f;
		lightUV.y = -v2lProj.y / v2lProj.w / 2.f + .5f;
		float vDepth = (v2lProj.z / v2lProj.w) - aBias;
		//float vDepth = ((length(fragToLight) / aRange) / v2lProj.w) - aBias;
		if (saturate(lightUV.x) == lightUV.x && saturate(lightUV.y) == lightUV.y && w2lView.z > 0)
		{
			float lDepth = aShadowMap.Sample(pointClampSampler, fragToLight).r;
			if (lDepth < vDepth) return true;
		}
	}
	return false;
}

float ResolveShadow(LightData light, Texture2D aShadowMap, float3 projectedTexCoord, float interleavedGradientNoise)
{
    static const int vogelSampleCount = 16;

    if (light.SmoothShadows)
    {
        return SampleShadowsVogelDisk(aShadowMap, projectedTexCoord, interleavedGradientNoise, light.VogalDiskScale, vogelSampleCount);
    }
    else
    {
        return aShadowMap.SampleLevel(pointClampSampler, projectedTexCoord.xy, 0) >= projectedTexCoord.z;
    }
}

float ResolveShadowCube(LightData light, TextureCube aShadowMap, float projectedDepth, float3 fragToLight, float4 worldToLightView, float interleavedGradientNoise)
{
    static const int vogelSampleCount = 16;

    if (light.SmoothShadows)
    {
        return SampleShadowCubeVogelDisk(aShadowMap, projectedDepth, worldToLightView, light.Range, fragToLight, interleavedGradientNoise, light.VogalDiskScale, vogelSampleCount);
    }
    else
    {
        float depth = aShadowMap.SampleLevel(pointClampSampler, fragToLight, 0).r;
        if (depth < projectedDepth)
        {
            return saturate(worldToLightView.z / (light.Range * light.Range));
        }
        return 1.f;
    }
}

float4 GetViewPosition(float2 uv)
{
    const float4 worldPosition = float4(ambientOcclusionTexture.Sample(defaultSampler, uv).rgb, 1);
    const float4 viewPosition = mul(FB_ToView, worldPosition);
    return viewPosition;
}

float4 GetViewNormal(float2 uv)
{
    const float4 worldNormal = float4(normalTexture.Sample(defaultSampler, uv).rgb, 0);
    const float4 viewNormal = mul(FB_ToView, worldNormal);
    return viewNormal;
}

float2 GetRandom(float2 uv, float2 uvScale)
{
    const float3 random = 2.0f * blueNoiseTexture.Sample(pointWrapSampler, uv * uvScale).rgb - 1.0f;
    return random.xy;
}
#endif