#define MAX_MODEL_BONES 128
SamplerState defaultSampler			:	register(s0);
SamplerState shadowSampler			:	register(s1);
SamplerState pointWrapSampler		:	register(s2);
SamplerState pointClampSampler		:	register(s3);

Texture2D albedoTexture				:	register(t0);
Texture2D normalTexture				:	register(t1);
Texture2D materialTexture			:	register(t2);

Texture2D vertexNormalTexture		:	register(t3);
Texture2D worldPositionTexture		:	register(t4);
Texture2D ambientOcclusionTexture	:	register(t5);
Texture2D SSAOTexture				:	register(t6);
Texture2D blueNoiseTexture			:	register(t8);
TextureCube environmentTexture		:	register(t10);

Texture2D dirLightShadowMap			:	register(t19);
Texture2D shadowMap[20]				:	register(t20);

TextureCube shadowCubeTexture[20]	:	register(t40);


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

struct PixelOutput
{
	float4 myColor			:	SV_TARGET;
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

cbuffer LineCBuffer : register(b2)
{
	float4x4 LCB_ToWorld;
	float4x4 LCB_ToView;
	float4x4 LCB_ToProjection;
};
