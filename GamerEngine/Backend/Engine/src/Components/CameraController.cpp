
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
	//AddComponent<CameraControllerData>();
}

void CameraController::OnDestroy()
{
	//ScriptableEntity::OnDestroy();
}

void CameraController::OnUpdate(GamerEngine::CameraComponent* aCamera, GamerEngine::TransformComponent* aTransform, float aTime)
{
	auto& camera = aCamera;

	if (!aCamera->Primary)
	{
		return;
	}

	const float aDeltaTime = Time::GetDeltaTime();

	Vector3f movement;

	HasBeenActivated = false;

	if (Input::IsMouseDown(VK_LBUTTON) && IsHoveringSceneView)
	{
		HasBeenActivated = true;
		Input::LockMouse(CommonUtilities::Mouse::Lock_CurrentPos);
		//myOldPos = myPrevMouse;
	}
	else
	{
		Input::LockMouse(CommonUtilities::Mouse::None);
	}

	if (Input::IsMouseReleased(VK_LBUTTON) && HasBeenActivated)
	{
		
		HasBeenActivated = false;
		//Input::SetMousePos(myPrevMouse);
	}

	if (!HasBeenActivated)
	{
		return;
	}

	const float oldMouseX = aTransform->GetRotation().x;
	const float oldMouseY = aTransform->GetRotation().y;




	Vector2f delta = Input::GetMouseDelta();
	//
	//SIZE windowRect = GraphicsEngine::Get()->GetWindowSize();
	//Vector2i centerPosition;
	//centerPosition.x = (windowRect.cx) / 2;
	//centerPosition.y = (windowRect.cy) / 2;
	//Input::SetMousePos(centerPosition);


	aTransform->SetRotation({
		aTransform->GetRotation().x + static_cast<float>(delta.y) * myMouseSensitivity,
		aTransform->GetRotation().y + static_cast<float>(delta.x) * myMouseSensitivity,
		0
	});
	//myOldPos = centerPosition;

	if (Input::GetMouseWheel() != 0.0f)
	{
		mySpeed += (Input::GetMouseWheel() * 0.5f) * (mySpeed * 0.25f);
		mySpeed = CommonUtilities::Clamp(mySpeed, 0.1f, myMaxSpeed);
	}

	float speed = mySpeed;

	

	if(Input::IsKeyDown(VK_SHIFT))
	{
		speed *= mySpeedShiftMul;
	}

	camera->SetCameraSpeed(speed);
	
	if(Input::IsKeyDown(87))
	{
		movement = 1.0f * aTransform->GetForward() * aDeltaTime * speed;
	}
	 if(Input::IsKeyDown(83))
	 {
	 	movement += -1.0f * aTransform->GetForward() * aDeltaTime * speed;
	 }
	 if(Input::IsKeyDown(68))
	 {
	 	movement += 1.0f * aTransform->GetRight() * aDeltaTime * speed;
	 }
	
	 if(Input::IsKeyDown(65))
	 {
	 	movement += -1.0f * aTransform->GetRight() * aDeltaTime * speed;
	 }
	
	if(Input::IsKeyDown(VK_SPACE))
	{
		movement += 1.0f * aTransform->GetUp() * aDeltaTime * speed;
	}

	if(Input::IsKeyDown(90))
	{
		movement += -1.0f * aTransform->GetUp() * aDeltaTime * speed;
	}

	if (movement != Vector3f::Zero() || oldMouseX != aTransform->GetRotation().x || oldMouseY != aTransform->GetRotation().y)
	{
		camera->SetHasMoved(true);
	}

	aTransform->SetPosition(aTransform->GetPosition() + movement);

	
}
