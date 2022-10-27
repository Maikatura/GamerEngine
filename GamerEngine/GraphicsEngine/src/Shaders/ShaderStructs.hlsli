struct VertexInput
{
	float4 myPosition		:	POSITION;
	float4 myVxColor		:	COLOR0;
	float4 myVxColor2		:	COLOR1;
	float4 myVxColor3		:	COLOR2;
	float4 myVxColor4		:	COLOR3;
	float2 myUV				:	TEXCOORD0;
	float2 myUV2			:	TEXCOORD1;
	float2 myUV3			:	TEXCOORD2;
	float2 myUV4			:	TEXCOORD3;
	uint4  myBoneIDs		:	BONEIDS;
	float4 myBoneWeights	:	BONEWEIGHTS;
	float3 myTangent		:	TANGENT;
	float3 myBinormal		:	BINORMAL;
	float3 myNormal			:	NORMAL;
};

struct VertexToPixel
{
	float4 myPosition		:	SV_POSITION;
	float4 myVxPosition		:	POSITION;
	float4 myVxColor		:	COLOR0;
	float4 myVxColor2		:	COLOR1;
	float4 myVxColor3		:	COLOR2;
	float4 myVxColor4		:	COLOR3;
	float2 myUV				:	TEXCOORD0;
	float2 myUV2			:	TEXCOORD1;
	float2 myUV3			:	TEXCOORD2;
	float2 myUV4			:	TEXCOORD3;
	float3 myTangent		:	TANGENT;
	float3 myBinormal		:	BINORMAL;
	float3 myNormal			:	NORMAL;
};

struct PixelOutput
{
	float4 myColor			:	SV_TARGET;
	float  myID				:	SV_TARGET1;
};

struct LineInput
{
	float4 myPosition		:	POSITION;
	float4 myColor			:	COLOR;
	float4 myWidth			:	WIDTH;
};

struct LineToPixel
{
	float4 myPosition		:	SV_POSITION;
	float4 myColor			:	COLOR;
};

struct GBufferOutput
{
	float4 Albedo			:	SV_TARGET0;
	float4 Normal			:	SV_TARGET1;
	float4 Material			:	SV_TARGET2;
	float4 VertexNormal		:	SV_TARGET3;
	float4 WorldPosition	:	SV_TARGET4;
	float AmbientOcclusion	:	SV_TARGET5;
};

cbuffer FrameBuffer	: register(b0)
{
	float4x4 FB_ToView;
	float4x4 FB_ToProjection;
	float3 FB_CamTranslation;
	int FB_RenderMode;
}

cbuffer Objectbuffer : register(b1)
{
	float4x4 OB_ToWorld;
	int OB_HasBones = 0;
	float3 OB_Padding;
	int OB_ID;
	float3 OB_Paddinging;
	float4x4 OB_BoneData[128];
}

cbuffer LightBuffer : register(b3)
{
	float3 LB_Color;
	float LB_Intensity;
	float3 LB_Direction;
	float LB_padding;
}

cbuffer LineCBuffer : register(b4)
{
	float4x4 LCB_ToWorld;
	float4x4 LCB_ToView;
	float4x4 LCB_ToProjection;
};

SamplerState defaultSampler : register(s0);

Texture2D albedoTexture		: register(t0);
Texture2D normalTexture		: register(t1);
Texture2D materialTexture	: register(t2);

TextureCube environmentTexture : register(t10);