#include "../Data/Textures.hlsli"
#include "../Data/ShaderStructs.hlsli"
#include "../Data/Samplers.hlsli"

PixelOutput main(FullscreenVertexToPixel input)
{
    PixelOutput output;

	float3 color1 = albedoTexture.Sample(defaultSampler, input.UV).rgb;
	float3 color2 = normalTexture.Sample(defaultSampler, input.UV).rgb;
	output.Color.rgb = (color1 + color2);
	output.Color.a = 1.0f;

	return output;
}