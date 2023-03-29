#pragma once
#include <Components/ScriptableEntity.h>


struct CameraControllerData
{
	float myMaxSpeed = 1000.0f;
	float mySpeed = 30.0f;
	float mySpeedShiftMul = 1.5f;
	float myMouseSensitivity = 0.033f;
	bool HasBeenActivated = false;
};

class CameraController : public ScriptableEntity
{


public:

	CameraController();

	inline static bool IsHoveringSceneView = false;

	float GetSpeed() const;
	float GetSpeedMul() const;
	float GetMaxSpeed() const;


	void SetSpeed(float aSpeed);
	void SetSpeedMul(float aSpeedMultiply);
	void SetMaxSpeed(float aMaxSpeed);

	void OnUpdate() override;

private:

	
};
