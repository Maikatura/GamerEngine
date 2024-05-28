#include "GraphicsEngine.pch.h"
#include "ScriptGlue.h"
#include "ScriptEngine.h"
#include "Components/AllComponents.h"
#include "Components/Components.hpp"
#include "Core/KeyCodes.h"

#include "Input/Input.h"

#include "Scene/Scene.h"
#include "Core/Model/Entity.h"


#include "mono/metadata/object.h"
#include "mono/metadata/reflection.h"
#include "ScriptRegister.h"


namespace GamerEngine {

	namespace Utils {

		std::string MonoStringToString(MonoString* string)
		{
			char* cStr = mono_string_to_utf8(string);
			std::string str(cStr);
			mono_free(cStr);
			return str;
		}

	}

	static std::unordered_map<MonoType*, std::function<bool(Entity)>> s_EntityHasComponentFuncs;

#define GE_ADD_INTERNAL_CALL(Name) mono_add_internal_call("GamerEngine.InternalCalls::" #Name, Name)

	static void NativeLog(MonoString* string, int parameter)
	{
		std::string str = Utils::MonoStringToString(string);
		std::cout << str << ", " << parameter << std::endl;
	}

	static void NativeLog_Vector(Vector3f* parameter, Vector3f* outResult)
	{
		GE_WARN("Value: {0}", *parameter);
		*outResult = Vector3f::Normalize(*parameter);
	}

	static float NativeLog_VectorDot(Vector3f* parameter)
	{
		GE_WARN("Value: {0}", *parameter);
		return Vector3f::Dot(*parameter, *parameter);
	}

	static MonoObject* GetScriptInstance(UUID entityID)
	{
		return ScriptEngine::GetManagedInstance(entityID);
	}

	static bool Entity_HasComponent(UUID entityID, MonoReflectionType* componentType)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		GE_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(entityID);
		GE_ASSERT(entity);

		MonoType* managedType = mono_reflection_type_get_type(componentType);
		GE_ASSERT(s_EntityHasComponentFuncs.find(managedType) != s_EntityHasComponentFuncs.end());
		return s_EntityHasComponentFuncs.at(managedType)(entity);
	}

	static bool Entity_GetComponent(UUID entityID, MonoReflectionType* componentType)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		GE_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(entityID);
		GE_ASSERT(entity);

		MonoType* managedType = mono_reflection_type_get_type(componentType);
		GE_ASSERT(s_EntityHasComponentFuncs.find(managedType) != s_EntityHasComponentFuncs.end());
		return s_EntityHasComponentFuncs.at(managedType)(entity);
	}

	static uint64_t Entity_FindEntityByName(MonoString* name)
	{
		char* nameCStr = mono_string_to_utf8(name);

		Scene* scene = ScriptEngine::GetSceneContext();
		GE_ASSERT(scene);
		Entity entity = scene->FindEntityByName(nameCStr);
		mono_free(nameCStr);

		if (!entity)
			return 0;
		
		return entity.GetUUID();
	}

	

	

	template<typename... Component>
	static void RegisterComponent()
	{
		([]()
		{
			std::string_view typeName = typeid(Component).name();
			size_t pos = typeName.find_last_of(':');
			std::string_view structName = typeName.substr(pos + 1);
			std::string managedTypename = std::format("GamerEngine.{}", structName);

			MonoType* managedType = mono_reflection_type_from_name(managedTypename.data(), ScriptEngine::GetCoreAssemblyImage());
			if (!managedType)
			{
				GE_LOG_ERROR("Could not find component type {}", managedTypename);
				return;
			}
			s_EntityHasComponentFuncs[managedType] = [](Entity entity) { return entity.HasComponent<Component>(); };
		}(), ...);
	}

	template<typename... Component>
	static void RegisterComponent(ComponentGroup<Component...>)
	{
		RegisterComponent<Component...>();
	}

	void ScriptGlue::RegisterComponents()
	{
		s_EntityHasComponentFuncs.clear();
		RegisterComponent(AllComponents{});
	}

	void ScriptGlue::RegisterFunctions()
	{
		GE_ADD_INTERNAL_CALL(NativeLog);
		GE_ADD_INTERNAL_CALL(NativeLog_Vector);
		GE_ADD_INTERNAL_CALL(NativeLog_VectorDot);

		GE_ADD_INTERNAL_CALL(GetScriptInstance);

		GE_ADD_INTERNAL_CALL(Entity_HasComponent);
		GE_ADD_INTERNAL_CALL(Entity_FindEntityByName);


		GE_ADD_INTERNAL_CALL(TransformComponent_GetTranslation);
		GE_ADD_INTERNAL_CALL(TransformComponent_SetTranslation);
		GE_ADD_INTERNAL_CALL(TransformComponent_SetRotation);
		GE_ADD_INTERNAL_CALL(TransformComponent_GetRotation);
		GE_ADD_INTERNAL_CALL(TransformComponent_GetScale);
		GE_ADD_INTERNAL_CALL(TransformComponent_SetScale);

		GE_ADD_INTERNAL_CALL(TransformComponent_GetForward);
		GE_ADD_INTERNAL_CALL(TransformComponent_GetRight);
		GE_ADD_INTERNAL_CALL(TransformComponent_GetUp);
		
		GE_ADD_INTERNAL_CALL(Input_IsKeyDown);
		GE_ADD_INTERNAL_CALL(Input_IsKeyUp);
		GE_ADD_INTERNAL_CALL(Input_IsKeyPressed);
	}

}