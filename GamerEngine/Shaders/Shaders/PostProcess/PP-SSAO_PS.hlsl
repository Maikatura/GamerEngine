#include "PostProcessStructs.hlsli"
#include "../Data/ShaderStructs.hlsli"
#include "../Data/Textures.hlsli"
#include "../Data/PBRFunctions.hlsli"

float SSAO(float2 aScreenTexCoord, float2 aTexCoord, float3 aViewPosition, float3 aViewNormal, float aScale, float aBias, float aIntensity)
{
	const float3 diff = viewPositionTexture.Sample(pointClampSampler, aScreenTexCoord + aTexCoord).rgb - aViewPosition;
	const float3 v = normalize(diff);
	const float d = length(diff) * aScale;
	const float occlusion = max(0.0f, dot(aViewNormal, v) - aBias) * 1.0f / (1.0f + d) * aIntensity;
	return occlusion;
}

DeferredPixelOutput main(DeferredVertexToPixel aInput)
{
	DeferredPixelOutput output;

	// SSAO SETTINGS
	float intensity = 1.0f;
	float scale = 0.5f;
	float bias = 0.15f;
	float radius = 0.310f;
	float offset = 0.6f;

	float2 randomTexCoordScale = aInput.myUV;
	const float random = (int)fmod(randomTexCoordScale.x, 4) + (int)(fmod(randomTexCoordScale.y, 4) * 4);

	//point clamp sampler since the textures have already been linearly interpolated when downsized
	const float3 viewPosition = viewPositionTexture.Sample(pointClampSampler, aInput.myUV).xyz;
	const float3 viewNormal = viewNormalTexture.Sample(pointClampSampler, aInput.myUV).xyz;

	float occlusion = 0.0f;

	for(uint i = 0; i < 4; i++)
	{
		const float2 coord1 = reflect(normalize(FB_FrustrumCorners[i % 4]).xy, random).xy * radius;
		const float2 coord2 = float2(coord1.x * offset - coord1.y * offset, coord1.x * offset + coord1.y * offset);

		occlusion += SSAO(aInput.myUV, coord1 * 0.25f, viewPosition, viewNormal, scale, bias, intensity);
		occlusion += SSAO(aInput.myUV, coord2 * 0.50f, viewPosition, viewNormal, scale, bias, intensity);
		occlusion += SSAO(aInput.myUV, coord1 * 0.75f, viewPosition, viewNormal, scale, bias, intensity);
		occlusion += SSAO(aInput.myUV, coord2 * 1.0f, viewPosition, viewNormal, scale, bias, intensity);
	}

	output.myColor.rgb = 1.0f - saturate(occlusion);
	output.myColor.a = 1.0f;
	return output;
}
