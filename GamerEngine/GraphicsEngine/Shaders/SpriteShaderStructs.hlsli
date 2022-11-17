struct VertexData
{
	float3 myPosition		:	POSITION;
	float myZRotation		:	ROTATION;
	float2 myScale			:	SCALE;
	float2 myPivot			:	CENTER;
	float4 myColor			:	COLOR;
};

struct GeometryToPixel
{
	float4 myPosition		:	SV_POSITION;
	float4 myColor			:	COLOR;
	float2 myUV				:   TEXCOORD;
};

struct PixelOutput
{
	float4 myColor			:	SV_TARGET;
	float myID				:	SV_TARGET1;
};

cbuffer FrameBuffer	: register(b0)
{
	float4x4 FB_ToView;
	float4x4 FB_ToProjection;
}

cbuffer Objectbuffer : register(b1)
{
	float4x4 OB_ToWorld;
	float4x4 OB_BoneData[128];
	bool OB_HasBones;
	int OB_ID;
	float2 OB_Padding;
}

SamplerState defaultSampler		:	register(s0);
Texture2D particleTexture		:	register(t0);