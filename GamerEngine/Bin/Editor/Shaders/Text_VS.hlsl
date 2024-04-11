#include "Data/ShaderStructs.hlsli"

float FmaSupported(float a, float b, float c)
{
    return a * b + c;
}

RectVertexOutput main(RectVertexInput input)
{
    const float2 RECT_POSITIONS[6] =
    {
        float2(-1.0f, -1.0f),
		float2(-1.0f, 1.0f),
		float2(1.0f, 1.0f),

		float2(1.0f, 1.0f),
		float2(1.0f, -1.0f),
		float2(-1.0f, -1.0f)
    };

    const float2 TEX_COORDS[6] =
    {
        float2(0, 0),
		float2(0, 1),
		float2(1, 1),
	
		float2(1, 1),
		float2(1, 0),
		float2(0, 0)
    };
	
    RectVertexOutput result;
    result.Position = float4(input.Position + RECT_POSITIONS[input.Index] * input.Size, 0, 1.f);
    result.TextureCoordinates = float2(
		FmaSupported(TEX_COORDS[input.Index].x, input.TexLayout.z, input.TexLayout.x),
		1.f - FmaSupported(TEX_COORDS[input.Index].y, input.TexLayout.w, input.TexLayout.y));
    result.Color = input.Color;
    result.ImageIndex = input.ImageIndex;
    result.SamplingData = input.SamplingData;
	
    return result;
}
