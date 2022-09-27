#pragma once
#include <Components/Components.hpp>
#include <Components/ScriptableEntity.h>

class Rotater : public ScriptableEntity
{
	void OnUpdate() override
	{
		auto& transform = GetComponent<TransformComponent>();
		transform.Rotation.x += Time::GetDeltaTime() * 50.f;
		transform.Rotation.y += Time::GetDeltaTime() * 50.f;
		//transform.Rotation.z += Time::GetDeltaTime() * 50.f;
	}
};
