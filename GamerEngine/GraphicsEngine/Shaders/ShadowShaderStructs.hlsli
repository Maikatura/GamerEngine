struct ShadowGeometryToPixel
{
    float4 Position : SV_POSITION;
    float4 FragPos : POSITION;
    uint Slice : SV_RenderTargetArrayIndex;
};

struct ShadowPixelOut
{
    float Depth : SV_Depth;
};

cbuffer PointLightViews : register(b5)
{
    float4x4 PLViews[6];
};