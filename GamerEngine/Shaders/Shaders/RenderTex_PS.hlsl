#include "Data/ShaderStructs.hlsli"
#include "Data/Textures.hlsli"
#include "Data/Samplers.hlsli"

DeferredPixelOutput main(DeferredVertexToPixel input)
{
	DeferredPixelOutput result;

	const float4 albedo = albedoTexture.Sample(defaultSampler, input.myUV);

	result.myColor = albedo;
	result.myColor.a = 1.0f;
	return result;
}
