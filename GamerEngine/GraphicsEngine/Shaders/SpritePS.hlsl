#include "SpriteShaderStructs.hlsli"

PixelOutput main(GeometryToPixel input)
{
	PixelOutput result;
	float4 texColor = particleTexture.Sample(defaultSampler, input.myUV);
	if(texColor.a < .05f) discard;
	result.myColor = texColor * input.myColor;
	return result;
}