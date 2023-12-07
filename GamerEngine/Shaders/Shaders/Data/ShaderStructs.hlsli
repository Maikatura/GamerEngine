#ifndef SHADERSTRUCTS_HLSLI
#define SHADERSTRUCTS_HLSLI

#define MAX_MODEL_BONES 512
#define BLENDSHAPE_ARRAY_SIZE 1024

struct VertexInput
{
	float4 Position		:	POSITION;

    float3 Normal : NORMAL;
	float3 Tangent : TANGENT;
    float3 Binormal : BINORMAL;

	float4 VxColor		:	COLOR0;
	float4 VxColor2		:	COLOR1;
	float4 VxColor3		:	COLOR2;
	float4 VxColor4		:	COLOR3;

	float2 UV				:	TEXCOORD0;
	float2 UV2			:	TEXCOORD1;
	float2 UV3			:	TEXCOORD2;
	float2 UV4			:	TEXCOORD3;

	uint4  BoneIDs		:	BONEIDS;
	float4 BoneWeights	:	BONEWEIGHTS;
	

	// Instancing Properties
	float4x4 Offset			:	WORLD;
	uint InstanceId			:	SV_InstanceID;
};

struct VertexToPixel
{
	float4 Position		:	SV_POSITION;
	float3 VertexWorldPosition	:	POSITION0;
	float3 ViewPosition	:	POSITION1;
    float3 Normal : NORMAL;
    float3 Tangent : TANGENT;
    float3 Binormal : BINORMAL;
	float4 VxColor		:	COLOR0;
	float4 VxColor2		:	COLOR1;
	float4 VxColor3		:	COLOR2;
	float4 VxColor4		:	COLOR3;
	float2 UV				:	TEXCOORD0;
	float2 UV2			:	TEXCOORD1;
	float2 UV3			:	TEXCOORD2;
	float2 UV4			:	TEXCOORD3;
    
};

struct PixelOutput
{
	float4 Color			:	SV_TARGET;
};

struct LineInput
{
	float4 Position		:	POSITION;
	float4 Color			:	COLOR;
	float4 Width			:	WIDTH;
};

struct LineToPixel
{
	float4 Position		:	SV_POSITION;
	float4 Color			:	COLOR;
};

struct GBufferOutput
{
	float4 Albedo			:	SV_TARGET0;
	float4 Normal			:	SV_TARGET1;
	float4 Material			:	SV_TARGET2;
	float4 VertexNormal		:	SV_TARGET3;
	float4 WorldPosition	:	SV_TARGET4;
	float AmbientOcclusion	:	SV_TARGET5;
	float4 ViewPosition		:	SV_TARGET6;
	float4 ViewNormal		:	SV_TARGET7;
};

struct FullscreenVertexInput
{
	unsigned int Index : SV_VERTEXID;
};

struct FullscreenVertexToPixel
{
	float4 Position		:	SV_POSITION;
	float2 UV				:	TEXCOORD;
};

struct DeferredPixelOutput
{
	float4 Color			:	SV_TARGET;
};

struct RectVertexInput
{
    unsigned int Index : SV_VERTEXID;
    float4 TexLayout : TEXLAYOUT;
    float4 Color : COLOR;
    float2 Position : POSITION;
    float2 Size : SIZE;
    uint ImageIndex : IMAGEINDEX;
    uint SamplingData : MSDFDATA;
};

struct RectVertexOutput
{
    float4 Position : SV_POSITION;
    float4 Color : COLOR;
    float2 TextureCoordinates : TEXCOORD0;
    uint ImageIndex : IMAGEINDEX;
    uint SamplingData : MSDFDATA;
};


cbuffer FrameBuffer	: register(b0)
{
	float4x4 FB_ToView;
	float4x4 FB_ToProjection;
	float3 FB_CamTranslation;
	unsigned int FB_RenderMode;
	float FB_NearPlane;
	float FB_FarPlane;
	float FB_DeltaTime;
	float FB_TotalTime;
	float2 FB_Resolution;
	float2 Padding;
	float4 FB_FrustrumCorners[4];
}

cbuffer Objectbuffer : register(b1)
{
	float4x4 OB_ToWorld;
	float4x4 OB_ToWorldInverse;
	bool OB_HasBones = false;
	unsigned int OB_ObjectId;
	bool OB_IsSelected = false;
	bool OB_IsInstanced = false;
	float4x4 OB_BoneData[MAX_MODEL_BONES];
}

cbuffer Materialbuffer : register(b2)
{
	float4 MB_Color;
}

cbuffer LineCBuffer : register(b6)
{
    float4x4 LCB_ToWorld;
    float4x4 LCB_ToView;
    float4x4 LCB_ToProjection;
};
#endif