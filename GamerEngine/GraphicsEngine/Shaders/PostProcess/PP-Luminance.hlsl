#include "../DeferredShaderStructs.hlsli"
#include "../ShaderStructs.hlsli"

DeferredPixelOutput main(DeferredVertexToPixel input)
{
	DeferredPixelOutput result;

	float4 color = albedoTexture.Sample(defaultSampler, input.myUV);

	if (color.a < 0.05f)
	{
		discard;
		result.myColor.rgb = float3(0, 0, 0);
		result.myColor.a = 0;
		return result;
	}
	const float3 resource = color.rgb;

	{
		float luminance = dot(resource, float3(0.2126f, 0.7152f, 0.0722f));
		/*float cutOff = 0.8f;
		if (luminance >= cutOff)
		{
			result.myColor.rgb = resource;
		}
		else
		{
			result.myColor.rgb = 0;
		}*/

		result.myColor.rgb = resource * pow(luminance, 5);
	}

	return result;
}