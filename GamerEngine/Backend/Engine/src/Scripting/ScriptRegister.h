#pragma once

#include "ScriptEngine.h"
#include "Components/TransfromComponent.h"
#include "Utilites/KeyCodes.h"
#include "Input/Input.h"
#include "Scene/Scene.h"

namespace GamerEngine
{



	static void TransformComponent_GetTranslation(UUID entityID, Vector3f* outTranslation)
	{
		GamerEngine::Scene* scene = GamerEngine::ScriptEngine::GetSceneContext();
		GE_ASSERT(scene);
		GamerEngine::Entity entity = scene->GetEntityByUUID(entityID);
		GE_ASSERT(entity);

		*outTranslation = entity.GetComponent<GamerEngine::TransformComponent>().GetPosition();
	}

	static void TransformComponent_SetTranslation(UUID entityID, Vector3f* translation)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		GE_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(entityID);
		GE_ASSERT(entity);

		entity.GetComponent<GamerEngine::TransformComponent>().SetPosition(*translation);
	}


	static void TransformComponent_GetRotation(UUID entityID, Vector3f* outTranslation)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		GE_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(entityID);
		GE_ASSERT(entity);

		*outTranslation = entity.GetComponent<GamerEngine::TransformComponent>().GetRotation();
	}

	static void TransformComponent_SetRotation(UUID entityID, Vector3f* translation)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		GE_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(entityID);
		GE_ASSERT(entity);

		entity.GetComponent<GamerEngine::TransformComponent>().SetRotation(*translation);
	}

	static void TransformComponent_GetScale(UUID entityID, Vector3f* outTranslation)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		GE_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(entityID);
		GE_ASSERT(entity);

		*outTranslation = entity.GetComponent<GamerEngine::TransformComponent>().GetScale();
	}

	static void TransformComponent_SetScale(UUID entityID, Vector3f* translation)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		GE_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(entityID);
		GE_ASSERT(entity);

		entity.GetComponent<GamerEngine::TransformComponent>().SetScale(*translation);
	}

	static void TransformComponent_GetForward(UUID entityID, Vector3f* outTranslation)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		GE_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(entityID);
		GE_ASSERT(entity);

		*outTranslation = entity.GetComponent<GamerEngine::TransformComponent>().GetForward();
	}

	static void TransformComponent_GetRight(UUID entityID, Vector3f* outTranslation)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		GE_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(entityID);
		GE_ASSERT(entity);

		*outTranslation = entity.GetComponent<GamerEngine::TransformComponent>().GetRight();
	}

	static void TransformComponent_GetUp(UUID entityID, Vector3f* outTranslation)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		GE_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(entityID);
		GE_ASSERT(entity);

		*outTranslation = entity.GetComponent<GamerEngine::TransformComponent>().GetUp();
	}


	static bool Input_Keyboard_IsKeyDown(uint16_t keycode)
	{
		return Input::IsKeyDown(static_cast<CommonUtilities::Key::Code>(keycode));
	}

	static bool Input_Keyboard_IsKeyUp(uint16_t keycode)
	{
		return Input::IsKeyReleased(static_cast<CommonUtilities::Key::Code>(keycode));
	}

	static bool Input_Keyboard_IsKeyPressed(uint16_t keycode)
	{
		return Input::IsKeyPressed(static_cast<CommonUtilities::Key::Code>(keycode));
	}

	static bool Input_Mouse_IsKeyDown(uint16_t keycode)
	{
		return Input::IsMouseDown(static_cast<CommonUtilities::Mouse::Button>(keycode));
	}

	static void Input_Mouse_LockMouse(uint16_t aMouseLockState)
	{
		Input::LockCursor(static_cast<CommonUtilities::CursorLockMode>(aMouseLockState));
	}

	static void Input_Mouse_GetDelta(Vector2f* outTranslation)
	{
		*outTranslation = Input::GetMouseDelta();
	}

	static float Input_Mouse_GetScrollWheel()
	{
		return Input::GetMouseWheel();
	}

}