#include "../Data/BufferData.hlsli"
#include "../Data/DeferredShaderStructs.hlsli"
#include "../Data/Samplers.hlsli"

DeferredPixelOutput main(DeferredVertexToPixel input)
{
	DeferredPixelOutput output;

	float3 color1 = albedoTexture.Sample(defaultSampler, input.myUV).rgb;
	float3 color2 = normalTexture.Sample(defaultSampler, input.myUV).rgb;
	output.myColor.rgb = (color1 + color2);
	output.myColor.a = 1.0f;

	return output;
}