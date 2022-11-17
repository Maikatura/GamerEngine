#include <Components/CameraController.h>
#include "Camera.h"
#include "Components/Components.hpp"
#include <Input/InputInternal.h>
#include <imgui/imgui.h>

#include "UtilityFunctions.h"
#include "Input/Input.h"


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
	camera.ViewProjection = Matrix4x4f::BuildTransform(transform.Translation, transform.Rotation, transform.Scale);

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

	transform.Rotation.y += InputInternal::GetMouseDelta().x * aDeltaTime * cameraData.myMouseSensitivity;
	transform.Rotation.x += InputInternal::GetMouseDelta().y * aDeltaTime * cameraData.myMouseSensitivity;

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

	if(InputInternal::IsKeyDown(ImGuiKey_LeftCtrl))
	{
		movement += { 0.0f, -1.0f * aDeltaTime * speed, 0.0f };
	}

	transform.Translation += movement;
}
