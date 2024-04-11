
#include "GraphicsEngine.pch.h"
#include <Components/CameraController.h>

#include "Time.hpp"
#include "Input/Input.h"
#include "Scene/SceneManager.h"
#include "Components/TransfromComponent.h"
#include "ImGui/ImGui.h"


CameraController::CameraController()
{

}

void CameraController::OnCreate()
{
	AddComponent<CameraControllerData>();
}

void CameraController::OnDestroy()
{
	ScriptableEntity::OnDestroy();
}

void CameraController::OnUpdate(float aTime)
{
	auto& camera = GetComponent<CameraComponent>();

	if(!camera.Primary)
	{
		return;
	}

	const float aDeltaTime = Time::GetDeltaTime();

	Vector3f movement;



	TransformComponent& transform = GetComponent<TransformComponent>();
	CameraControllerData& cameraData = GetComponent<CameraControllerData>();

	if (Input::IsMousePressed(VK_LBUTTON) && IsHoveringSceneView)
	{
		cameraData.HasBeenActivated = true;
		myPrevMouse = Input::GetMousePos();
		myOldPos = myPrevMouse;
	}

	if(Input::IsMouseReleased(VK_LBUTTON) && cameraData.HasBeenActivated)
	{
		cameraData.HasBeenActivated = false;
		Input::SetMousePos(myPrevMouse);
	}

	if(!cameraData.HasBeenActivated)
	{
		return;
	}

	const float oldMouseX = transform.GetRotation().x;
	const float oldMouseY = transform.GetRotation().y;


	

	Vector2i delta = Input::GetMousePos() - myOldPos;

	SIZE windowRect = GraphicsEngine::Get()->GetWindowSize();
	Vector2i centerPosition;
	centerPosition.x = (windowRect.cx) / 2;
	centerPosition.y = (windowRect.cy) / 2;
	Input::SetMousePos(centerPosition);


	transform.GetRotation().y += static_cast<float>(delta.x) * cameraData.myMouseSensitivity;
	transform.GetRotation().x += static_cast<float>(delta.y) * cameraData.myMouseSensitivity;
	myOldPos = centerPosition;

	if (Input::GetMouseWheel() != 0.0f)
	{
		cameraData.mySpeed += (Input::GetMouseWheel() * 0.5f) * (cameraData.mySpeed * 0.25f);
		cameraData.mySpeed = CommonUtilities::Clamp(cameraData.mySpeed, 0.1f, cameraData.myMaxSpeed);
	}

	float speed = cameraData.mySpeed;

	

	if(Input::IsKeyDown(VK_SHIFT))
	{
		speed *= cameraData.mySpeedShiftMul;
	}

	camera.SetCameraSpeed(speed);
	
	if(Input::IsKeyDown(87))
	{
		movement = 1.0f * transform.GetForward() * aDeltaTime * speed;
	}
	 if(Input::IsKeyDown(83))
	 {
	 	movement += -1.0f * transform.GetForward() * aDeltaTime * speed;
	 }
	 if(Input::IsKeyDown(68))
	 {
	 	movement += 1.0f * transform.GetRight() * aDeltaTime * speed;
	 }
	
	 if(Input::IsKeyDown(65))
	 {
	 	movement += -1.0f * transform.GetRight() * aDeltaTime * speed;
	 }
	
	if(Input::IsKeyDown(VK_SPACE))
	{
		movement += 1.0f * transform.GetUp() * aDeltaTime * speed;
	}

	if(Input::IsKeyDown(90))
	{
		movement += -1.0f * transform.GetUp() * aDeltaTime * speed;
	}

	if (movement != Vector3f::Zero() || oldMouseX != transform.GetRotation().x || oldMouseY != transform.GetRotation().y)
	{
		camera.SetHasMoved(true);
	}

	transform.GetPosition() += movement;

	
}
