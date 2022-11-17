#include "../DeferredShaderStructs.hlsli"
#include "../ShaderStructs.hlsli"

DeferredPixelOutput main(DeferredVertexToPixel input)
{
	DeferredPixelOutput result;

	float4 color1 = albedoTexture.Sample(defaultSampler, input.myUV);
	float4 color2 = normalTexture.Sample(defaultSampler, input.myUV);
	result.myColor = color1 + color2;

	return result;
}
