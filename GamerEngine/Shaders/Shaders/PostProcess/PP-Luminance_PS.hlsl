#include "../Data/Textures.hlsli"
#include "../Data/ShaderStructs.hlsli"
#include "../Data/Samplers.hlsli"

DeferredPixelOutput main(DeferredVertexToPixel input)
{
	DeferredPixelOutput output;



	float4 color = albedoTexture.Sample(defaultSampler, input.myUV);
	float luminance = dot(color.rgb, float3(.2f, .75f, .6f));
	float cutOff = .82f;
	luminance = saturate(luminance - cutOff);
	output.myColor = color * luminance * (1.0f / cutOff);

	return output;
}