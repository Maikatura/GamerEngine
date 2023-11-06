#include "../Data/Textures.hlsli"
#include "../Data/ShaderStructs.hlsli"
#include "../Data/Samplers.hlsli"

DeferredPixelOutput main(DeferredVertexToPixel input)
{
	DeferredPixelOutput output;

	float3 color1 = albedoTexture.Sample(defaultSampler, input.UV).rgb;
	float3 color2 = normalTexture.Sample(defaultSampler, input.UV).rgb;
	output.Color.rgb = (color1 + color2);
	output.Color.a = 1.0f;

	return output;
}