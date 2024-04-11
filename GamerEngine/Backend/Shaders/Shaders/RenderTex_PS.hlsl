#include "Data/ShaderStructs.hlsli"
#include "Data/Textures.hlsli"
#include "Data/Samplers.hlsli"

FullscreenPixelOutput main(FullscreenVertexToPixel input)
{
    FullscreenPixelOutput result;

	const float4 albedo = albedoTexture.Sample(defaultSampler, input.UV);

	result.Color = albedo;
	result.Color.a = 1.0f;

    result.Depth = fullscreenDepth.SampleLevel(pointClampSampler, input.UV, 0.0f).r;

	return result;
}
