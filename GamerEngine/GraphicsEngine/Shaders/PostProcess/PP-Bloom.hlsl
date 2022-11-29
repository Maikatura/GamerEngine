#include "../Data/DeferredShaderStructs.hlsli"

DeferredPixelOutput main(DeferredVertexToPixel input)
{
	DeferredPixelOutput output;

	float4 color1 = albedoTexture.Sample(defaultSampler, input.myUV);
	float4 color2 = normalTexture.Sample(defaultSampler, input.myUV);
	output.myColor = (color1 + color2);

	return output;
}