#include "Editor.pch.h"
#include <Components/CameraController.h>
#include "Components/Components.hpp"
#include <Input/InputInternal.h>

#include "Time.hpp"
#include "Utilites/UtilityFunctions.h"
#include "Input/Input.h"
#include "Scene/SceneManager.h"
#include "Components/TransfromComponent.h"


CameraController::CameraController() : ScriptableEntity(true)
{

}

void CameraController::OnUpdate()
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

	if(InputInternal::IsMousePressed(VK_LBUTTON) && IsHoveringSceneView)
	{
		cameraData.HasBeenActivated = true;
	}

	if(InputInternal::IsMouseReleased(VK_LBUTTON))
	{
		cameraData.HasBeenActivated = false;
	}

	if(!cameraData.HasBeenActivated)
	{
		return;
	}

	const float oldMouseX = transform.GetRotation().x;
	const float oldMouseY = transform.GetRotation().y;

	

	transform.GetRotation().y += Input::GetMouseDelta().x * cameraData.myMouseSensitivity;
	transform.GetRotation().x += Input::GetMouseDelta().y * cameraData.myMouseSensitivity;
	

	if (ImGui::GetIO().MouseWheel != 0.0f)
	{
		cameraData.mySpeed += (ImGui::GetIO().MouseWheel * 0.5f) * (cameraData.mySpeed * 0.25f);
		cameraData.mySpeed = CommonUtilities::Clamp(cameraData.mySpeed, 0.1f, cameraData.myMaxSpeed);
	}

	float speed = cameraData.mySpeed;

	

	if(InputInternal::IsKeyDown(ImGuiKey_LeftShift))
	{
		speed *= cameraData.mySpeedShiftMul;
	}

	camera.SetCameraSpeed(speed);
	
	if(InputInternal::IsKeyDown(ImGuiKey_W))
	{
		movement = 1.0f * transform.GetForward() * aDeltaTime * speed;
	}
	if(InputInternal::IsKeyDown(ImGuiKey_S))
	{
		movement += -1.0f * transform.GetForward() * aDeltaTime * speed;
	}
	if(InputInternal::IsKeyDown(ImGuiKey_D))
	{
		movement += 1.0f * transform.GetRight() * aDeltaTime * speed;
	}

	if(InputInternal::IsKeyDown(ImGuiKey_A))
	{
		movement += -1.0f * transform.GetRight() * aDeltaTime * speed;
	}

	if(InputInternal::IsKeyDown(ImGuiKey_Space))
	{
		movement += 1.0f * transform.GetUp() * aDeltaTime * speed;
	}

	if(InputInternal::IsKeyDown(ImGuiKey_Z))
	{
		movement += -1.0f * transform.GetUp() * aDeltaTime * speed;
	}

	if (movement != Vector3f::Zero() || oldMouseX != transform.GetRotation().x || oldMouseY != transform.GetRotation().y)
	{
		camera.SetHasMoved(true);
	}

	transform.GetPosition() += movement;

	
}
