#pragma once

#include <model/Entity.h>

class ScriptableEntity
{
public:
	virtual ~ScriptableEntity() {}

	template<typename T>
	T& GetComponent()
	{
		return myEntity.GetComponent<T>();
	}

	template<typename T>
	ScriptableEntity& GetScriptComponents()
	{
		return myEntity->GetScriptComponent<T>();
	}

	template<typename T>
	bool HasComponent() const
	{
		return myEntity.HasComponent<T>();
	}

	template<typename T, typename... Args>
	T& AddComponent(Args&&... args)
	{
		return myEntity.AddComponent<T>(args);
	}

	template<typename T, typename... Args>
	T& AddOrReplaceComponent(Args&&... args)
	{
		return myEntity.AddOrReplaceComponent<T>(args);
	}

	template<typename T>
	void RemoveComponent()
	{
		myEntity.RemoveComponent<T>();
	}


protected:
	virtual void OnCreate() {}
	virtual void OnDestroy() {}
	virtual void OnUpdate() {}
	virtual void OnRender() {}

private:
	Entity myEntity;
	friend class Scene;
};