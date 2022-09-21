#pragma once
#include <Math/Vector2.hpp>
#include <Math/Matrix4x4.hpp>
#include <Math/Vector3.hpp>
#include "Model/SceneObject.h"

class Camera : public SceneObject
{
	CommonUtilities::Matrix4x4<float> myProjection = {};
	

	CommonUtilities::Vector3<float> myPos = { 0.0f,0.0f,0.0f };
	CommonUtilities::Vector3<float> myFocusPos = { 0.0f,0.0f,0.0f };
	CommonUtilities::Vector3<float> myUp = { 0.0f,1.0f, 0.0f };
	CommonUtilities::Vector3<float> myRight = { 1.0f, 0.0f, 0.0f };

	float myYaw = 0.0f;
	float myPitch = 0.0f;

	float myNearPlane = 0.1f;
	float myFarPlane = 1500.0f;
	float myFoV = 90.0f;

	float myCameraSensitivity = 10.0f;
	float mySpeed = 10.0f;

	CommonUtilities::Vector2<float> myLastMousePos;

public:
	Camera() : SceneObject()
	{
		
	}

	bool Initialize(float aHorizontalFoV, CommonUtilities::Vector2<unsigned int> aResolution, float aNearPlane, float aFarPlane);
	
	void Resize(CommonUtilities::Vector2<unsigned int> aResolution);

	void SetPosition(CommonUtilities::Vector3<float> aPosition);
	
	CommonUtilities::Vector3<float> GetPosition();

	CommonUtilities::Vector3<float> GetXAxis();
	CommonUtilities::Vector3<float> GetYAxis();
	CommonUtilities::Vector3<float> GetZAxis();

	void Update();

	FORCEINLINE CommonUtilities::Matrix4x4<float> const& GetProjectionMatrix() const { return myProjection; }
	FORCEINLINE CommonUtilities::Matrix4x4<float>& GetProjectionMatrixMutable() { return myProjection; }
	FORCEINLINE float GetNearPlane() const { return myNearPlane; }
	FORCEINLINE float GetFarPlane() const { return myFarPlane; }
};

