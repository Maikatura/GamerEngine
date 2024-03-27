#include "PostProcessStructs.hlsli"
#include "../Data/ShaderStructs.hlsli"
#include "../Data/Textures.hlsli"
#include "../Data/PBRFunctions.hlsli"

#define SSAO_NOISE_SIZE 4
#define SSAO_NOISE_COUNT (SSAO_NOISE_SIZE * SSAO_NOISE_SIZE)

float SSAO(float2 aScreenTexCoord, float2 aTexCoord, float3 aViewPosition, float3 aViewNormal, float aScale, float aBias, float aIntensity)
{
    const float3 diff = viewPositionTexture.Sample(pointClampSampler, aScreenTexCoord + aTexCoord).rgb - aViewPosition;
    const float3 v = normalize(diff);
    const float d = length(diff) * aScale;
    const float occlusion = max(0.0f, dot(aViewNormal, v) - aBias) * 1.0f / (1.0f + d) * aIntensity;
    return occlusion;
}

PixelOutput main(FullscreenVertexToPixel aInput)
{
    PixelOutput output;

	// SSAO SETTINGS
	float intensity = 1.0f;
	float scale = 0.5f;
	float bias = 0.2f;
	float radius = 0.310f;
	float offset = 0.6f;

	float2 randomTexCoordScale = aInput.UV * FB_Resolution;
	const float random = (int)fmod(randomTexCoordScale.x, 4) + (int)(fmod(randomTexCoordScale.y, 4) * 4);

	//point clamp sampler since the textures have already been linearly interpolated when downsized
	const float3 viewPosition = viewPositionTexture.Sample(pointWrapSampler, aInput.UV).xyz;
    const float3 viewNormal = viewNormalTexture.Sample(pointWrapSampler, aInput.UV).xyz;

	float occlusion = 0.0f;


	for(uint i = 0; i < 4; i++)
	{
		const float2 coord1 = reflect(normalize(FB_FrustrumCorners[i % 4]).xy, random).xy * radius;
		const float2 coord2 = float2(coord1.x * offset - coord1.y * offset, coord1.x * offset + coord1.y * offset);

		occlusion += SSAO(aInput.UV, coord1 * 0.25f, viewPosition, viewNormal, scale, bias, intensity);
		occlusion += SSAO(aInput.UV, coord2 * 0.50f, viewPosition, viewNormal, scale, bias, intensity);
		occlusion += SSAO(aInput.UV, coord1 * 0.75f, viewPosition, viewNormal, scale, bias, intensity);
		occlusion += SSAO(aInput.UV, coord2 * 1.0f, viewPosition, viewNormal, scale, bias, intensity);
	}

	output.Color.rgb = 1.0f - saturate(occlusion);
	output.Color.a = 1.0f;
	return output;
}
