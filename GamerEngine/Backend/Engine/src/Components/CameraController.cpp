
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

	if (Input::IsMouseDown(CommonUtilities::Mouse::RightButton) && IsHoveringSceneView)
	{
		HasBeenActivated = true;
		Input::LockCursor(CommonUtilities::CursorLockMode::Locked);
		Input::SetCursorVisibility(false);
		//myOldPos = myPrevMouse;
	}
	else
	{
		if (Input::IsMouseReleased(CommonUtilities::Mouse::RightButton) && HasBeenActivated)
		{
			HasBeenActivated = false;
		}

		Input::SetCursorVisibility(true);
		Input::LockCursor(CommonUtilities::CursorLockMode::None);
	}

	if (!HasBeenActivated)
	{
		return;
	}

	const float oldMouseX = aTransform->GetRotation().x;
	const float oldMouseY = aTransform->GetRotation().y;


	Vector2f delta = Input::GetMouseDelta();



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

	

	if(Input::IsKeyDown(CommonUtilities::Key::Code::LeftShift))
	{
		speed *= mySpeedShiftMul;
	}

	camera->SetCameraSpeed(speed);
	
	if(Input::IsKeyDown(CommonUtilities::Key::Code::W))
	{
		movement = 1.0f * aTransform->GetForward() * aDeltaTime * speed;
	}
	 if(Input::IsKeyDown(CommonUtilities::Key::Code::S))
	 {
	 	movement += -1.0f * aTransform->GetForward() * aDeltaTime * speed;
	 }
	 if(Input::IsKeyDown(CommonUtilities::Key::Code::D))
	 {
	 	movement += 1.0f * aTransform->GetRight() * aDeltaTime * speed;
	 }
	
	 if(Input::IsKeyDown(CommonUtilities::Key::Code::A))
	 {
	 	movement += -1.0f * aTransform->GetRight() * aDeltaTime * speed;
	 }
	
	if(Input::IsKeyDown(CommonUtilities::Key::Code::Space))
	{
		movement += 1.0f * aTransform->GetUp() * aDeltaTime * speed;
	}

	if(Input::IsKeyDown(CommonUtilities::Key::Code::Z))
	{
		movement += -1.0f * aTransform->GetUp() * aDeltaTime * speed;
	}

	if (movement != Vector3f::Zero() || oldMouseX != aTransform->GetRotation().x || oldMouseY != aTransform->GetRotation().y)
	{
		camera->SetHasMoved(true);
	}

	aTransform->SetPosition(aTransform->GetPosition() + movement);

	
}
