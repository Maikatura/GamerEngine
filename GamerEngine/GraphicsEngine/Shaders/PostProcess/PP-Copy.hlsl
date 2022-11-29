#include "../Data/DeferredShaderStructs.hlsli"

DeferredPixelOutput main(DeferredVertexToPixel input)
{
	DeferredPixelOutput output;

	float4 color = albedoTexture.Sample(defaultSampler, input.myUV);
	output.myColor = color;

	return output;
}