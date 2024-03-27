#pragma once
#include "Core/Model/ScriptableEntity.h"

struct CameraControllerData
{
	CameraControllerData() = default;
	CameraControllerData(const CameraControllerData&) = default;
	
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
	
	void OnCreate() override;
	void OnDestroy() override;
	void OnUpdate(float aTime) override;



private:
	Vector2i myPrevMouse {};
	Vector2i myOldPos {};

	
};
