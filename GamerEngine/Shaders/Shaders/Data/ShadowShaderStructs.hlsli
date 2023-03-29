struct ShadowGeometryToPixel
{
    float4 Position : SV_POSITION;
    float4 FragPos : POSITION;
    uint Slice : SV_RenderTargetArrayIndex;
    float3 Normal : NORMAL;
    float2 UV : TEXCOORD0;
};

struct ShadowPixelOut
{
    float Depth : SV_Depth;
};

cbuffer PointLightViews : register(b5)
{
    float3 SC_LightTranslation;
    float SC_FarPlane;
    float4x4 SC_Projection;
    float4x4 PLViews[6];
};