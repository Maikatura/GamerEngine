#include "Data/ShaderStructs.hlsli"
#include "Data/SpriteShaderStructs.hlsli"
#include "Data/BufferData.hlsli"
#include "Data/Samplers.hlsli"

SpritePixelOutput main(SpriteGeometryToPixel input)
{
	SpritePixelOutput result;
	float4 texColor = particleTexture.Sample(defaultSampler, input.myUV);
	if(texColor.a < .05f) discard;
	result.myColor = texColor * input.myColor;
	return result;
}