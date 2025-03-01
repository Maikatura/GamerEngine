#include "../Data/Textures.hlsli"
#include "../Data/ShaderStructs.hlsli"
#include "../Data/Samplers.hlsli"

PixelOutput main(FullscreenVertexToPixel input)
{
    PixelOutput output;

	float4 color = albedoTexture.Sample(pointClampSampler, input.UV);
	output.Color.rgb = color.rgb;
	output.Color.a = 1.0f;

	return output;
}