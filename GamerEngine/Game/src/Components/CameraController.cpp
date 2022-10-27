#include <Components/CameraController.h>
#include "Camera.h"
#include "Components/Components.hpp"
#include <Input/InputInternal.h>
#include <imgui/imgui.h>

#include "UtilityFunctions.h"
#include "Input/Input.h"


float CameraController::GetSpeed() const
{
	return mySpeed;
}

float CameraController::GetSpeedMul() const
{
	return mySpeedShiftMul;
}

float CameraController::GetMaxSpeed() const
{
	return myMaxSpeed;
}

void CameraController::SetSpeed(float aSpeed)
{
	mySpeed = aSpeed;
}

void CameraController::SetSpeedMul(float aSpeedMultiply)
{
	mySpeedShiftMul = aSpeedMultiply;
}

void CameraController::SetMaxSpeed(float aMaxSpeed)
{
	myMaxSpeed = aMaxSpeed;
}

void CameraController::OnUpdate()
{
	auto& camera = GetComponent<CameraComponent>();

	if(!camera.Primary)
	{
		return;
	}

	auto& transform = GetComponent<TransformComponent>();
	auto transformMatrix = transform.GetMatrix();

	float aDeltaTime = Time::GetDeltaTime();


	Vector3f movement;
	camera.ViewProjection = Matrix4x4f::BuildTransform(transform.Translation, transform.Rotation, transform.Scale);



	if(InputInternal::IsMousePressed(VK_LBUTTON) && IsHoveringSceneView)
	{
		HasBeenActivated = true;
	}

	if(InputInternal::IsMouseReleased(VK_LBUTTON))
	{
		HasBeenActivated = false;
	}

	if(!HasBeenActivated)
	{
		return;
	}

	transform.Rotation.y += InputInternal::GetMouseDelta().x * aDeltaTime * myMouseSensitivity;
	transform.Rotation.x += InputInternal::GetMouseDelta().y * aDeltaTime * myMouseSensitivity;

	if (ImGui::GetIO().MouseWheel != 0.0f)
	{
		mySpeed += (ImGui::GetIO().MouseWheel * 0.5f) * (mySpeed * 0.25f);
		mySpeed = CommonUtilities::Clamp(mySpeed, 0.1f, myMaxSpeed);
	}

	float speed = mySpeed;

	if(InputInternal::IsKeyDown(ImGuiKey_LeftShift))
	{
		speed *= mySpeedShiftMul;
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
