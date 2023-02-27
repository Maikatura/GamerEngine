#include "PostProcessStructs.hlsli"
#include "../Data/DeferredShaderStructs.hlsli"
#include "../Data/BufferData.hlsli"
#include "../Data/PBRFunctions.hlsli"

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

float SSAO(float2 screenUV, float2 uv, float3 pxViewPos, float3 pxViewNrm, float scale, float bias, float intensity)
{
	const float3 diff	= GetViewPosition(screenUV + uv).xyz - pxViewPos;
	const float3 v		= normalize(diff);
	const float d		= length(diff) * scale;
	const float occ		= max(0.0f, dot(pxViewNrm, v) - bias) * 1.0f / (1.0f + d) * intensity;

	return occ;
}

DeferredPixelOutput main(DeferredVertexToPixel input)
{
	DeferredPixelOutput result;

	// SSAO SETTINGS
	float intensity		= 1.0f;
	float scale			= 0.5f;
	float bias			= 0.15f;
	float radius		= 0.310f;
	float offset		= 0.6f;

	int noiseWidth = 0;
	int noiseHeight = 0;
	int numMipmaps = 0;
	blueNoiseTexture.GetDimensions(0, noiseWidth, noiseHeight, numMipmaps);
	float2 randomUVScale = FB_Resolution / float2(noiseWidth, noiseHeight);
	float2 random = GetRandom(input.myUV, randomUVScale);

	const float4 pxPos = GetViewPosition(input.myUV);
	const float4 pxNrm = GetViewNormal(input.myUV);

	/*float2 frustrumCorners[4] = { FB_FrustrumCorners[0], FB_FrustrumCorners[1],FB_FrustrumCorners[2],FB_FrustrumCorners[3] };*/

	float occlusion = 0.0f;
	for (int i = 0; i < 4; i++)
	{
		const float2 coord1 = reflect(normalize(FB_FrustrumCorners[i % 4]).xy, random) * radius;
		const float2 coord2 = float2(coord1.x * offset - coord1.y * offset, coord1.x * offset + coord1.y * offset);

		occlusion += SSAO(input.myUV, coord1 * 0.25f, pxPos.xyz, pxNrm.xyz, scale, bias, intensity);
		occlusion += SSAO(input.myUV, coord2 * 0.50f, pxPos.xyz, pxNrm.xyz, scale, bias, intensity);
		occlusion += SSAO(input.myUV, coord1 * 0.75f, pxPos.xyz, pxNrm.xyz, scale, bias, intensity);
		occlusion += SSAO(input.myUV, coord2 * 1.00f, pxPos.xyz, pxNrm.xyz, scale, bias, intensity);
	}

	result.myColor.rgb = 1.0f - saturate(occlusion);
	result.myColor.a = 1;
	return result;
}
