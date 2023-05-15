#pragma once
#include <Math/MathTypes.hpp>


enum class BlendState
{
	None,
	Alpha,
	Additive,
	BSCount
};

enum class DepthStencilState
{
	ReadWrite,
	ReadOnly,
	ReadOnlyLEqual,
	StencilKeep,
	StencilZero,
	Disabled,
	Count
};

enum class SamplerState
{
	None,
	Default,
	PointClamp,
	Wrap,
	PointWrap,
	LinearClamp,
	LinearWrap,
	ComparisonLinearClamp,
	Count
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
