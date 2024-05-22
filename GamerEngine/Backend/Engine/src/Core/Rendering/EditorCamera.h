#pragma once
#include "Components/CameraController.h"



class CameraController;

namespace GamerEngine
{
	class CameraComponent;
	class TransformComponent;
}


class EditorCamera
{
	bool myFirstInit = false;

public:
	GamerEngine::CameraComponent* myCamera;
	GamerEngine::TransformComponent* myTransform;
	CameraController* myCameraController;


	EditorCamera();

	~EditorCamera();

	void Update();
};
