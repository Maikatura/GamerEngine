#include "ShadowShaderStructs.hlsli"
#include "ShaderStructs.hlsli"

[maxvertexcount(18)]
void main(
    triangle VertexToPixel input[3],
    inout TriangleStream<ShadowGeometryToPixel> output
)
{
    float4x4 lightView = float4x4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, FB_CamTranslation, 1);

    for(unsigned int face = 0; face < 6; ++face)
    {
        ShadowGeometryToPixel result;
        result.Slice = face;

        lightView = PLViews[face];

        for(unsigned int index = 0; index < 3; ++index)
        {
            result.FragPos = input[index].myVxPosition;
            const float4 worldToLightView = mul(lightView, input[index].myVxPosition);
            const float4 lightViewToLightProj = mul(FB_ToProjection, worldToLightView);
            result.Position = lightViewToLightProj;
            output.Append(result);
        }
        output.RestartStrip();
    }
}
