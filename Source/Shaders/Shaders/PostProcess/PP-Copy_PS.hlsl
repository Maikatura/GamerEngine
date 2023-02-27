#include "../Data/BufferData.hlsli"
#include "../Data/DeferredShaderStructs.hlsli"
#include "../Data/Samplers.hlsli"

DeferredPixelOutput main(DeferredVertexToPixel input)
{
	DeferredPixelOutput output;

	float4 color = albedoTexture.Sample(pointClampSampler, input.myUV);
	output.myColor.rgb = color.rgb;
	output.myColor.a = 1.0f;

	return output;
}