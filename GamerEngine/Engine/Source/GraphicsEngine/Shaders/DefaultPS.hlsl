#include "ShaderStructs.hlsli"

PixelOutput main(VertexToPixel input)
{
	PixelOutput result;

	result.myColor.rgb = albedoTexture.Sample(defaultSampler, input.myUV).rgb;
	result.myColor.a = 1;

	return result;
}