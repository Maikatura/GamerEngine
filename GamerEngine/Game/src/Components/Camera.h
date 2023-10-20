#pragma once
#include "Components/ScriptableEntity.h"
#include "Render/Renderer.h"
#include "Math/Frustum.h"
#define PI 3.14f

class Camera
{
	CommonUtilities::Frustum myCameraFrustum;
	Matrix4x4f myProjection;
	Matrix4x4f myViewProjection;

	float myNearPlane = 0.1f;
	float myFarPlane = 1500.0f;
	float myFoV = 90.0f;
	float myVFoV = 0.0f;

public:


	bool Initialize(float aHorizontalFoV, CommonUtilities::Vector2<unsigned int> aResolution, float aNearPlane, float aFarPlane)
	{
		assert(aNearPlane < aFarPlane);
		assert(aNearPlane >= 0.01f);

		myFoV = aHorizontalFoV;

		myNearPlane = aNearPlane;
		myFarPlane = aFarPlane;

		Resize(aResolution);

		return true;
	}
	void Resize(CommonUtilities::Vector2<unsigned int> aResolution)
	{
		myVFoV = myFoV * (PI / 180.f);
		const float vFoVRad = 2 * std::atan(std::tan(myVFoV / 2) * (static_cast<float>(aResolution.y) / static_cast<float>(aResolution.x)));

		const float myXScale = 1 / std::tanf(myVFoV * 0.5f);
		const float myYScale = 1 / std::tanf(vFoVRad * 0.5f);
		const float Q = myFarPlane / (myFarPlane - myNearPlane);

		myProjection(1, 1) = myXScale;
		myProjection(2, 2) = myYScale;
		myProjection(3, 3) = Q;
		myProjection(3, 4) = 1.0f / Q;
		myProjection(4, 3) = -Q * myNearPlane;
		myProjection(4, 4) = 0.0f;

		myCameraFrustum = CommonUtilities::CreateFrustumFromCamera(myViewProjection, myVFoV, myVFoV, myNearPlane, myFarPlane);

	}

	void SetViewMatrix(CommonUtilities::Matrix4x4<float> aViewMatrix) { myViewProjection = aViewMatrix; }

	FORCEINLINE CommonUtilities::Matrix4x4<float> const& GetViewMatrix() const { return myViewProjection; }
	FORCEINLINE CommonUtilities::Matrix4x4<float> const& GetProjectionMatrix() const { return myProjection; }
	FORCEINLINE float GetNearPlane() const { return myNearPlane; }
	FORCEINLINE float GetFarPlane() const { return myFarPlane; }


private:

};
