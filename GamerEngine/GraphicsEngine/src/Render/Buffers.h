#pragma once
#include <Math/MathTypes.hpp>


enum class BlendState
{
	NoBlend,
	AlphaBlend,
	AdditiveBlend,
	BSCount
};

enum class DepthStencilState
{
	DSS_ReadWrite,
	DSS_ReadOnly,
	DSS_Ignore,
	DSS_Overlay,
	DSS_Count
};

enum class SamplerState
{
	SS_Default,
	SS_Wrap,
	SS_PointClamp,
	SS_PointWrap,
	SS_Count
};

struct FrameBufferData
{
	Matrix4x4f View;
	Matrix4x4f Projection;
	Vector3f CamTranslation;
	unsigned int RenderMode;
	float NearPlane;
	float FarPlane;
	float DeltaTime;
	float TotalTime;
	Vector2ui Resolution;
	Vector2f Padding;
	Vector4f FrustrumCorners[4];
};

struct ObjectBufferData
{
	Matrix4x4f World;
	Matrix4x4f WorldInv;
	alignas(4) bool myHasBones = false;
	unsigned int OB_ObjectId;
	alignas(4) bool IsSelected = false;
	alignas(4) bool IsInstanced;
	Matrix4x4f BoneData[MAX_MODEL_BONES]{};
};