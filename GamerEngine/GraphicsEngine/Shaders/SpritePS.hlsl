#include "Data/ShaderStructs.hlsli"
#include "Data/SpriteShaderStructs.hlsli"

SpritePixelOutput main(SpriteGeometryToPixel input)
{
	PixelOutput result;
	float4 texColor = particleTexture.Sample(defaultSampler, input.myUV);
	if(texColor.a < .05f) discard;
	result.myColor = texColor * input.myColor;
	return result;
}