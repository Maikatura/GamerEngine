#include "Data/ShaderStructs.hlsli"

[maxvertexcount(4)]
void main(line LineToPixel input[2], inout TriangleStream<LineToPixel> OutputStream)
{
    LineToPixel result;

    // Calculate the direction of the line in 3D space
    float3 dir = normalize(input[1].WorldPos.xyz - input[0].WorldPos.xyz);

    // Calculate the normal of the quad using the cross product of the line direction and the camera's view direction
    float3 normal = cross(dir, input[0].ViewDir);

    // Calculate the positions of the four vertices of the quad
    float3 quad[4];
    quad[0] = input[0].Position.xyz - normal * input[0].LineWidth / 2;
    quad[1] = input[0].Position.xyz + normal * input[0].LineWidth / 2;
    quad[2] = input[1].Position.xyz - normal * input[1].LineWidth / 2;
    quad[3] = input[1].Position.xyz + normal * input[1].LineWidth / 2;

    // Output the four vertices of the quad
    for (int i = 0; i < 4; ++i)
    {
        result.Position = float4(quad[i], input[0].Position.w);
        result.Color = input[0].Color;
        result.LineWidth = input[0].LineWidth;
        result.ViewDir = input[0].ViewDir;
        result.WorldPos = input[0].WorldPos;
        OutputStream.Append(result);
    }
}
