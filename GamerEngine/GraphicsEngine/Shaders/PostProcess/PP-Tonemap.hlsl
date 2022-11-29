#include "../Data/DeferredShaderStructs.hlsli"
#include "../Data/Tonemaps.hlsli"
#include "../Data/Common.hlsli"

DeferredPixelOutput main(DeferredVertexToPixel input)
{
	DeferredPixelOutput result;

	const float3 color = albedoTexture.Sample(defaultSampler, input.myUV).rgb;
	const uint mode = 2;

	switch (mode)
	{
		default:
			result.myColor.rgb = LinearToGamma(color);
			break;
		case 1:
			result.myColor.rgb = LinearToGamma(Tonemap_Reinhard2(color));
			break;
		case 2:
			result.myColor.rgb = Tonemap_UnrealEngine(color);
			break;
		case 3:
			result.myColor.rgb = LinearToGamma(Tonemap_ACES(color));
			break;
		case 4:
			result.myColor.rgb = LinearToGamma(Tonemap_Lottes(color));
			break;
	}

	result.myColor.a = 1;
	
	return result;
}