#include "../Data/ShaderStructs.hlsli"
#include "../Data/Textures.hlsli"
#include "../Data/Tonemaps.hlsli"
#include "../Data/Common.hlsli"
#include "../Data/Samplers.hlsli"

DeferredPixelOutput main(DeferredVertexToPixel input)
{
	DeferredPixelOutput result;

	const float4 color = albedoTexture.Sample(defaultSampler, input.myUV).rgba;
	const uint mode = 2;

	switch (mode)
	{
		default:
			result.myColor.rgb = LinearToSRGB(color.rgb);
			break;
		case 1:
			result.myColor.rgb = LinearToSRGB(Tonemap_Reinhard2(color.rgb));
			break;
		case 2:
			result.myColor.rgb = Tonemap_UnrealEngine(color.rgb);
			break;
		case 3:
			result.myColor.rgb = LinearToSRGB(Tonemap_ACES(color.rgb));
			break;
		case 4:
			result.myColor.rgb = LinearToSRGB(Tonemap_Lottes(color.rgb));
			break;
	}

	result.myColor.a = color.a;
	
	return result;
}