#include "../Data/ShaderStructs.hlsli"
#include "../Data/Textures.hlsli"
#include "../Data/Tonemaps.hlsli"
#include "../Data/Common.hlsli"
#include "../Data/Samplers.hlsli"

PixelOutput main(FullscreenVertexToPixel input)
{
    PixelOutput result;

	const float4 color = albedoTexture.Sample(defaultSampler, input.UV).rgba;
	const uint mode = 2;

	switch (mode)
	{
		default:
			result.Color.rgb = LinearToSRGB(color.rgb);
			break;
		case 1:
			result.Color.rgb = LinearToSRGB(Tonemap_Reinhard2(color.rgb));
			break;
		case 2:
			result.Color.rgb = Tonemap_UnrealEngine(color.rgb);
			break;
		case 3:
			result.Color.rgb = LinearToSRGB(Tonemap_ACES(color.rgb));
			break;
		case 4:
			result.Color.rgb = LinearToSRGB(Tonemap_Lottes(color.rgb));
			break;
	}

	result.Color.a = color.a;
	
	return result;
}