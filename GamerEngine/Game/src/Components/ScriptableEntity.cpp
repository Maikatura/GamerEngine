#include "ScriptableEntity.h"
#include "RandomMoverComponent.h"

ScriptableEntity::ScriptableEntity() : Component(false)
{}

ScriptableEntity::ScriptableEntity(bool aRunInEditor) : Component(aRunInEditor)
{}

template <typename T>
T* ScriptableEntity::GetScriptComponent()
{
	return myEntity->GetScriptComponent<T>();
}
