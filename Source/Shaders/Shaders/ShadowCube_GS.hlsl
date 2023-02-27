#include "Data/ShadowShaderStructs.hlsli"
#include "Data/ShaderStructs.hlsli"

[maxvertexcount(18)]
void main(
    triangle VertexToPixel input[3],
    inout TriangleStream<ShadowGeometryToPixel> output
)
{
    
    for(unsigned int face = 0; face < 6; ++face)
    {
        ShadowGeometryToPixel result;
        result.Slice = face;
        
        for(unsigned int index = 0; index < 3; ++index)
        {
            result.FragPos = float4(input[index].myVxPosition.xyz, 1.f);
            const float4 worldToLightView = mul(PLViews[face], float4(input[index].myVxPosition.xyz, 1.f));
            const float4 lightViewToLightProj = mul(FB_ToProjection, worldToLightView);
            result.Position = lightViewToLightProj;
            output.Append(result);
        }
        output.RestartStrip();
    }
}
