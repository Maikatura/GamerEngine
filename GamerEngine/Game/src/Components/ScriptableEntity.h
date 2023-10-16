#pragma once

#include "Renderer/model/Entity.h"

#include "Components.hpp"

class ScriptableEntity : public Component
{
public:
	ScriptableEntity();
	ScriptableEntity(bool aRunInEditor);

	virtual ~ScriptableEntity() {}

	template<typename T>
	T& GetComponent()
	{
		return myEntity.GetComponent<T>();
	}

	template<typename T>
	T* GetScriptComponent();

	template<typename T>
	bool HasComponent() const
	{
		return myEntity.HasComponent<T>();
	}

	template<typename T, typename... Args>
	T& AddComponent(Args&&... args)
	{
		return myEntity.AddComponent<T>(std::forward<Args>(args)...);
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

	UUID2 GetUUID() { return myEntity.GetUUID(); }

protected:
	void OnCreate() override {}
	void OnDestroy() override {}
	void OnUpdate() override {}
	void OnRender() override {}

private:
	Entity myEntity;
	friend class Scene;
	friend class SceneSerializer;
	friend struct NativeScriptComponent;
};

