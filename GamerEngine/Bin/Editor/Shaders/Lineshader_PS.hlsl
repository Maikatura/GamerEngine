#include "Data/ShaderStructs.hlsli"

PixelOutput main(LineToPixel input) : SV_TARGET
{
	PixelOutput result;
	result.Color = input.Color;

	return result;
}