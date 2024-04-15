#pragma once
#include <entt.hpp>

#include "Components/IDComponent.h"
#include "Scene/Scene.h"


namespace GamerEngine
{



	class Scene;

	class Entity
	{
	public:

		Entity() : myScene(nullptr)
		{ }

		Entity(entt::entity aHandle, Scene* aScene) : myEntityHandle(aHandle), myScene(aScene)
		{ }

		Entity(const Entity& other) = default;

		Entity operator=(const Entity& other)
		{
			myEntityHandle = other.myEntityHandle;
			myScene = other.myScene;

			return *this;
		}

		GamerEngine::UUID GetUUID() { return GetComponent<IDComponent>().ID; }

		template<typename T>
		T& AddComponent()
		{
			T& component = myScene->GetRegistry().emplace<T>(myEntityHandle);
			//myScene->OnComponentAdded<T>(*this, component);
			return component;
		}


		template<typename T>
		T& AddOrReplaceComponent()
		{
			T& component = myScene->GetRegistry().emplace_or_replace<T>(myEntityHandle);
			//myScene->OnComponentAdded<T>(*this, component);
			return component;
		}

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			T& component = myScene->GetRegistry().emplace<T>(myEntityHandle, std::forward<Args>(args)...);
			//myScene->OnComponentAdded<T>(*this, component);
			return component;
		}


		template<typename T, typename... Args>
		T& AddOrReplaceComponent(Args&&... args)
		{
			T& component = myScene->GetRegistry().emplace_or_replace<T>(myEntityHandle, std::forward<Args>(args)...);
			//myScene->OnComponentAdded<T>(*this, component);
			return component;
		}

		template<typename T>
		T& GetComponent()
		{
			return myScene->GetRegistry().get<T>(myEntityHandle);
		}

		template<typename T>
		bool HasComponent()
		{
			return myScene->GetRegistry().all_of<T>(myEntityHandle);
		}

		template<typename T>
		void RemoveComponent()
		{
			myScene->GetRegistry().remove<T>(myEntityHandle);
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

}


