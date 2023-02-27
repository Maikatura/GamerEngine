#pragma once
#include <entt.hpp>
#include <Renderer/Scene/Scene.h>

#include "Components/Components.hpp"
#include "Renderer/Scene/UUID.h"


class Entity
{
public:

	Entity() : myScene(nullptr)
	{ }

	Entity(entt::entity aHandle, Scene* aScene) : myEntityHandle(aHandle), myScene(aScene) 
	{ }

	Entity(const Entity& other) = default;

	UUID2 GetUUID() { return GetComponent<IDComponent>().ID; }

	template<typename T, typename... Args>
	T& AddComponent(Args&&... args)
	{
		T& component = myScene->myRegistry.emplace<T>(myEntityHandle, std::forward<Args>(args)...);
		//myScene->OnComponentAdded<T>(*this, component);
		return component;
	}

	template<typename T, typename... Args>
	T& AddOrReplaceComponent(Args&&... args)
	{
		T& component = myScene->myRegistry.emplace_or_replace<T>(myEntityHandle, std::forward<Args>(args)...);
		//myScene->OnComponentAdded<T>(*this, component);
		return component;
	}

	template<typename T>
	T* GetScriptComponent();

	template<typename T>
	T& GetComponent()
	{
		return myScene->myRegistry.get<T>(myEntityHandle);
	}

	template<typename T>
	bool HasComponent() const
	{
		return myScene->myRegistry.all_of<T>(myEntityHandle);
	}

	template<typename T>
	void RemoveComponent()
	{
		myScene->myRegistry.remove<T>(myEntityHandle);
	}

	operator bool() const { return myEntityHandle != entt::null; }
	operator entt::entity() const { return myEntityHandle; }
	operator uint32_t() const { return (uint32_t)myEntityHandle; }

	bool operator==(const Entity& other) const
	{
		return myEntityHandle == other.myEntityHandle && myScene == other.myScene;
	}

	bool operator!=(const Entity& other) const
	{
		return !(*this == other);
	}

	entt::entity GetHandle() const
	{
		if (myEntityHandle == entt::null)
		{
			return entt::null;
		}
		return myEntityHandle;
	}


	uint32_t GetID() const
	{
		return (uint32_t)myEntityHandle;
	}




private:

	entt::entity myEntityHandle{ entt::null };
	

	Scene* myScene = nullptr;
};


