#include "Data/ShaderStructs.hlsli"

PixelOutput main(LineToPixel input) : SV_TARGET
{
	PixelOutput result;
	result.myColor = input.myColor;

	return result;
}