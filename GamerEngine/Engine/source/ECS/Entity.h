#pragma once
#include "Scene.h"
#include <entt/entt.hpp>

namespace GamerEngine
{
	class Entity
	{
	public:
		Entity(entt::entity aEntityID, Scene* aScene);
		Entity(const Entity& other) = default;

		template<typename T, typename... Args>
		T& AddComponent (Args&&... args)
		{
			asset(!HasComponent<T>() && "Entity has component");
			return myScene->myRegistry.emplace<T>(myEntityID, std::forward<Args>(args)...);
		}

		template<typename T>
		T& GetComponent()
		{
			asset(HasComponent<T>() && "Entity does not have component");
			return myScene->myRegistry.get<T>(myEntityID);
		}

		template<typename T>
		bool HasComponent()
		{
			return myScene->myRegistry.has<T>(myEntityID);
		}

		template<typename T>
		void RemoveComponent()
		{
			asset(HasComponent<T>() && "Entity does not have component");
			myScene->myRegistry.remove<T>(myEntityID);
		}

	private:
		entt::entity myEntityID;
		Scene* myScene;
	};
}