#pragma once
#include "CameraComponent.h"


class CameraController
{


public:

	CameraController();

	inline static bool IsHoveringSceneView = false;
	
	void OnCreate();
	void OnDestroy();
	void OnUpdate(GamerEngine::CameraComponent* aCamera, GamerEngine::TransformComponent* aTransform, float aTime);



private:
	Vector2i myPrevMouse {};
	Vector2i myOldPos {};

	float myMaxSpeed = 1000.0f;
	float mySpeed = 30.0f;
	float mySpeedShiftMul = 1.5f;
	float myMouseSensitivity = 0.033f;
	bool HasBeenActivated = false;
};
