#pragma once
#include <Components/ScriptableEntity.h>

#include "Camera.h"
#include "Components/Components.hpp"
#include "Input/Input.h"
#include "Render/Renderer.h"

class CameraController : public ScriptableEntity
{

	float mySpeed = 30.0f;
	float mySpeedShiftMul = 1.5f;
	float myMouseSensitivity = 10.0f;

public:

	float& Speed() { return mySpeed; }
	float& SpeedMul() { return mySpeedShiftMul; }


	void OnUpdate() override
	{
		auto& camera = GetComponent<CameraComponent>();

		if (!camera.Primary)
		{
			return;
		}

		auto& transform = GetComponent<TransformComponent>();
		auto transformMatrix = transform.GetTransform();

		float aDeltaTime = Time::GetDeltaTime();
		
		
		Vector3f movement;
		camera.ViewProjection = Matrix4x4f::BuildTransform(transform.Translation, transform.Rotation, transform.Scale);

		if(!Input::IsMouseDown(VK_LBUTTON))
		{
			return;
		}

		transform.Rotation.y += Input::GetMouseDelta().x * aDeltaTime * myMouseSensitivity;
		transform.Rotation.x += Input::GetMouseDelta().y * aDeltaTime * myMouseSensitivity;

		float speed = mySpeed;

		if (Input::IsKeyDown(VK_SHIFT))
		{
			speed *= mySpeedShiftMul;
		}


		if(Input::IsKeyDown('W'))
		{
			movement = 1.0f * transformMatrix.GetZAxis() * aDeltaTime * speed;
		}
		if(Input::IsKeyDown('S'))
		{
			movement += -1.0f * transformMatrix.GetZAxis() * aDeltaTime * speed;
		}
		if(Input::IsKeyDown('D'))
		{
			movement += 1.0f * transformMatrix.GetXAxis() * aDeltaTime * speed;
		}

		if(Input::IsKeyDown('A'))
		{
			movement += -1.0f * transformMatrix.GetXAxis() * aDeltaTime * speed;
		}

		if(Input::IsKeyDown(VK_SPACE))
		{
			movement += { 0.0f, 1.0f * aDeltaTime * speed, 0.0f };
		}

		if(Input::IsKeyDown(VK_CONTROL))
		{
			movement += { 0.0f, -1.0f * aDeltaTime * speed, 0.0f };
		}

		transform.Translation += movement;
	}

	

private:

	
};
