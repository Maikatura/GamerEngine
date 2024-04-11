#include "Data/ShaderStructs.hlsli"
#include "Data/SpriteShaderStructs.hlsli"

[maxvertexcount(4)]
void main(
	point SpriteVertexData input[1],
	inout TriangleStream<SpriteGeometryToPixel> output
)
{
	float2 offsets[4] =
	{
		{-50.f, 50.f},
		{50.f, 50.f},
		{-50.f, -50.f},
		{50.f, -50.f}
	};
	float2 UVs[4] =
	{
		{0, 0},
		{1, 0},
		{0, 1},
		{1, 1}
	};

	SpriteVertexData pInput = input[0];
	pInput.myZRotation *= 3.14159f / 180.f;
	float2x2 rotMarix = float2x2(
		cos(pInput.myZRotation), -sin(pInput.myZRotation),
		sin(pInput.myZRotation), cos(pInput.myZRotation)
		);

	for(int i = 0; i < 4; i++)
	{
		SpriteGeometryToPixel result;
		float4 pos;
		float2 offs;

		pos.xyz = pInput.myPosition;
		pos.w = 1;
		result.myPosition = mul(FB_ToView, pos);
		offs = mul(rotMarix, offsets[i]);
		result.myPosition.xy += offs * pInput.myScale.xy;		//Rotation no work, don't know Y...
		//result.myPosition.xy += offsets[i] * pInput.myScale.xy;
		result.myPosition = mul(FB_ToProjection, result.myPosition);
		result.myColor = pInput.myColor;
		result.myUV = UVs[i];
		output.Append(result);
	}
}