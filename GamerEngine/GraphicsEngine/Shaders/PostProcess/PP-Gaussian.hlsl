#include "../Data/DeferredShaderStructs.hlsli"

DeferredPixelOutput main(DeferredVertexToPixel input)
{
	DeferredPixelOutput output;

	float4 color = albedoTexture.Sample(defaultSampler, input.myUV);
	float tau = 6.28318530718f;
	float directions = 7.f;
	float quality = 4.f;
	float radius = 5.f;
	const float2 rad = radius / float2(FB_Resolution.x, FB_Resolution.y);

	for (float d = 0.f; d < tau; d += tau / directions)
	{
		for (float i = 1.f / quality; i <= 1.f; i += 1.f / quality)
		{
			color += albedoTexture.Sample(defaultSampler, input.myUV + float2(cos(d), sin(d)) * rad * i);
		}
	}

	color /= quality * directions - (directions - 1);

	output.myColor = color;

	return output;
}