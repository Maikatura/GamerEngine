#pragma once
#include "Math/Vector2.hpp"
#include "Math/Matrix4x4.hpp"
#include "Model/SceneObject.h"

class Camera : public SceneObject
{
	CommonUtilities::Matrix4x4<float> myProjection = {};
	

	CommonUtilities::Vector3<float> myPos = { 0.0f,0.0f,0.0f };
	CommonUtilities::Vector3<float> myFocusPos = { 0.0f,0.0f,0.0f };
	CommonUtilities::Vector3<float> myUp = { 0.0f,1.0f, 0.0f };

	float myNearPlane = 0.1f;
	float myFarPlane = 1500.0f;

public:
	bool Initialize(float aHorizontalFoV, CommonUtilities::Vector2<unsigned int> aResolution, float aNearPlane, float aFarPlane);
	
	FORCEINLINE CommonUtilities::Matrix4x4<float> const& GetProjectionMatrix() const { return myProjection; }
	FORCEINLINE float GetNearPlane() const { return myNearPlane; }
	FORCEINLINE float GetFarPlane() const { return myFarPlane; }
};

