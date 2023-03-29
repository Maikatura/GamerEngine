#include "Editor.pch.h"
#include <Components/CameraController.h>
#include "Components/Camera.h"
#include "Components/Components.hpp"
#include <Input/InputInternal.h>

#include "Utilites/UtilityFunctions.h"
#include "Renderer/Input/Input.h"


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

	float aDeltaTime = Time::GetDeltaTime();

	Vector3f movement;

	TransformComponent& transform = GetComponent<TransformComponent>();
	CameraControllerData& cameraData = GetComponent<CameraControllerData>();

	auto transformMatrix = transform.GetMatrix();
	
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

	float oldMouseX = transform.Rotation.x;
	float oldMouseY = transform.Rotation.y;

	

	transform.Rotation.y += Input::GetMouseDelta().x * cameraData.myMouseSensitivity;
	transform.Rotation.x += Input::GetMouseDelta().y * cameraData.myMouseSensitivity;

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
		movement = 1.0f * transformMatrix.GetForward() * aDeltaTime * speed;
	}
	if(InputInternal::IsKeyDown(ImGuiKey_S))
	{
		movement += -1.0f * transformMatrix.GetForward() * aDeltaTime * speed;
	}
	if(InputInternal::IsKeyDown(ImGuiKey_D))
	{
		movement += 1.0f * transformMatrix.GetSide() * aDeltaTime * speed;
	}

	if(InputInternal::IsKeyDown(ImGuiKey_A))
	{
		movement += -1.0f * transformMatrix.GetSide() * aDeltaTime * speed;
	}

	if(InputInternal::IsKeyDown(ImGuiKey_Space))
	{
		movement += { 0.0f, 1.0f * aDeltaTime * speed, 0.0f };
	}

	if(InputInternal::IsKeyDown(ImGuiKey_Z))
	{
		movement += { 0.0f, -1.0f * aDeltaTime * speed, 0.0f };
	}

	if (movement != Vector3f::Zero() || oldMouseX != transform.Rotation.x || oldMouseY != transform.Rotation.y)
	{
		camera.SetHasMoved(true);
	}

	transform.Translation += movement;
}
