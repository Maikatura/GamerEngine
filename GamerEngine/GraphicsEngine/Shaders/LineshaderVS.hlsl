#include "Data/ShaderStructs.hlsli"

LineToPixel main(LineInput input)
{

	const float4x4 skinningMatrix = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };

	const float4 vertexWorldPosition = mul(LCB_ToWorld, mul(input.myPosition, skinningMatrix));
	const float4 vertexViewPosition = mul(LCB_ToView, vertexWorldPosition);
	const float4 vertexProjectionPosition = mul(LCB_ToProjection, vertexViewPosition);

	LineToPixel result;
	result.myColor = input.myColor;
	result.myPosition = vertexProjectionPosition;

	return result;
}