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
	SS_PointClamp,
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
};

struct ObjectBufferData
{
	Matrix4x4f World;
	int myHasBones;
	Vector3f myPadding1;
	int myObjectId = 0;
	Vector3f myPadding2;
	Matrix4x4f myBoneData[MAX_MODEL_BONES]{};
};