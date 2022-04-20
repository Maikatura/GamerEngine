#pragma once
#include "Scene.h"
#include <entt/entt.hpp>
#include <cassert>

namespace GamerEngine
{
	class Entity
	{
	public:
		Entity();
		Entity(entt::entity aEntityID, Scene* aScene);
		Entity(const Entity& other) = default;

		template<typename T, typename... Args>
		T& AddComponent (Args&&... args)
		{
			assert(!HasComponent<T>() && "Entity has component");
			return myScene->myRegistry.emplace<T>(myEntityID, std::forward<Args>(args)...);
		}

		template<typename T>
		T& GetComponent()
		{
			assert(HasComponent<T>() && "Entity does not have component");
			return myScene->myRegistry.get<T>(myEntityID);
		}

		template<typename T>
		bool HasComponent()
		{
			return false;
			return myScene->myRegistry.any_of<T>(myEntityID);
		}

		template<typename T>
		void RemoveComponent()
		{
			assert(HasComponent<T>() && "Entity does not have component");
			myScene->myRegistry.remove<T>(myEntityID);
		}

		bool IsNull()
		{
			return myEntityID == entt::null;
		}

	private:
		entt::entity myEntityID = entt::null;
		Scene* myScene;
	};
}