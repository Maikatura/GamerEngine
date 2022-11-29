#include "Data/DeferredShaderStructs.hlsli"

DeferredPixelOutput main(DeferredVertexToPixel input)
{
	DeferredPixelOutput result;

	const float4 albedo = albedoTexture.Sample(defaultSampler, input.myUV);

	result.myColor = albedo;
	return result;
}
