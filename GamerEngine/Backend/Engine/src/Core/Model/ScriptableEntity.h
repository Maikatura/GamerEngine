#pragma once

#include "Entity.h"

class ScriptableEntity
{
public:

	template<typename T>
	T& GetComponent()
	{
		return myEntity.GetComponent<T>();
	}


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

	virtual void OnCreate()
	{
		
	}

	virtual void OnDestroy()
	{
		
	}

	virtual void OnUpdate(float aTime)
	{
		
	}

	GamerEngine::UUID GetUUID() { return myEntity.GetUUID(); }


private:
	GamerEngine::Entity myEntity;
	friend class GamerEngine::Scene;
};



