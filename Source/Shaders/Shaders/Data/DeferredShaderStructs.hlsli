#define MAX_MODEL_BONES 128

struct DeferredVertexInput
{
	unsigned int myIndex	: SV_VERTEXID;
};

struct DeferredVertexToPixel
{
	float4 myPosition		:	SV_POSITION;
	float2 myUV				:	TEXCOORD;
};

struct DeferredPixelOutput
{
	float4 myColor			:	SV_TARGET;
};

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

	// Instancing Properties
	float4x4 Offset			:	WORLD;
	uint InstanceId			:	SV_InstanceID;
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
