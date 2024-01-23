#pragma once
#include "Math/Frustum.h"
#include "Math/MathTypes.hpp"
#include "Math/Ray.hpp"
#define PI 3.14f


struct GraphicsTransform
{
	Vector3f Translation{ 0.0f, 0.0f, 0.0f };
	Quatf Rotation{ 1.0f, 0.0f, 0.0f, 0.0f };
	Vector3f Scale{ 1.0f, 1.0f, 1.0f };

	GraphicsTransform() = default;
	GraphicsTransform(const GraphicsTransform&) = default;

	GraphicsTransform(const Vector3f& aTranslation, const Quatf& aRotation, const Vector3f& aScale)
		: Translation(aTranslation)
		, Rotation(aRotation)
		, Scale(aScale)
	{}

	Matrix4x4f GetMatrix() const
	{
		return ComposeFromTRS(Translation, {0,0,0}, Scale);
	}


	static GraphicsTransform Combine(const GraphicsTransform& aSecond, const GraphicsTransform& aFirst)
	{
		GraphicsTransform out;
		out.Scale = aSecond.Scale * aFirst.Scale;

		out.Rotation = aFirst.Rotation * aSecond.Rotation;

		out.Translation = aSecond.Rotation * (aSecond.Scale * aFirst.Translation);
		out.Translation = aSecond.Translation + out.Translation;

		return out;
	}

	void Mix(const GraphicsTransform& aTransToMixWith, float t)
	{
		const  Quatf& secondRot = aTransToMixWith.Rotation;
		Translation = Vector3f::Lerp(Translation, aTransToMixWith.Translation, t);
		Rotation = Quatf::Slerp(Rotation, secondRot, t);
		Scale = Vector3f::Lerp(Scale, aTransToMixWith.Scale, t);
	}

	static GraphicsTransform Mix(const GraphicsTransform& aFirstTransform, const GraphicsTransform& aSecondTransform, float aT)
	{
		GraphicsTransform resultTrans = aFirstTransform;
		resultTrans.Mix(aSecondTransform, aT);

		return resultTrans;
	}

	static GraphicsTransform Inverse(const GraphicsTransform& aTransform)
	{
		GraphicsTransform inv;
		inv.Rotation = Quatf::Inverse(aTransform.Rotation);
		inv.Scale.x = std::abs(aTransform.Scale.x) < 0.0001f ? 0.0f : 1.0f / aTransform.Scale.x;
		inv.Scale.y = std::abs(aTransform.Scale.y) < 0.0001f ? 0.0f : 1.0f / aTransform.Scale.y;
		inv.Scale.z = std::abs(aTransform.Scale.z) < 0.0001f ? 0.0f : 1.0f / aTransform.Scale.z;
		Vector3f invTrans = aTransform.Translation * -1.0f;
		inv.Translation = inv.Rotation * (inv.Scale * invTrans);
		return inv;
	}
};

class Camera
{
public:
	Camera() = default;
	void SetPerspective(float aHorizontalFoV, const Vector2ui& aResolution, float aNearClipPlane, float aFarClipPlane);
	void SetOrthographicPerspective(float aLeft, float aRight, float aBottom, float aTop, float aNear, float aFar);
	const Matrix4x4f& GetProjection() const { return myProjection; }
	float GetNearClipPlane() const { return myNearClipPlane; }
	float GetFarClipPlane() const { return myFarClipPlane; }
	float GetHFoV() const { return myHFoV; }
	float GetVFoV() const { return myVFoV; }
	float GetAspectRatio() const { return myAspectRatio; }
	float GetAspectRatioVertical() const { return 1.0f / myAspectRatio; }

	Matrix4x4f GetViewMatrix() const;
	Matrix4x4f GetWorldViewMatrix() const;
	const Vector2ui& GetResolution() { return myResolution; }

	GraphicsTransform& GetTransform() { return myTransform; }
	const GraphicsTransform& GetTransform() const { return myTransform; }

	Vector3f ScreenToWorldPos(Vector2f aMousePos, float aZ) const;
	CommonUtilities::Ray<float> ScreenToWorldRay(Vector2f aMousePos) const;

	const CommonUtilities::Frustum& GetFrustum() const;
private:
	Matrix4x4f myProjection{};
	GraphicsTransform myTransform{};
	CommonUtilities::Frustum myFrustum;
	Vector2ui myResolution{ 0, 0 };
	float myHFoV{ 30.0f };
	float myVFoV{ 0.0f }; //Calculated from HFoV
	float myAspectRatio{ 16.0f / 9.0f };
	float myNearClipPlane{ 0.3f };
	float myFarClipPlane{ 50000.0f };
};
