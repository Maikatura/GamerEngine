#pragma once
#include "ECS/Scene.h"
#include <entt/entt.hpp>
#include <cassert>

namespace GamerEngine
{
	class Scene;

	class Entity
	{
	public:
		Entity();
		Entity(entt::entity aEntityID, Scene* aScene);
		Entity(const Entity& other) = default;

		template<typename T, typename ...Args>
		T& AddComponent(Args && ...args)
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

		template <typename T>
		bool HasComponent()
		{
			if (myScene->myRegistry.valid(myEntityID))
			{
				return myScene->myRegistry.all_of<T>(myEntityID);
			}
			return false;
		}

		template <typename T>
		void RemoveComponent()
		{
			assert(HasComponent<T>() && "Entity does not have component");
			myScene->myRegistry.remove<T>(myEntityID);
		}

		bool operator==(Entity aEntity)
		{
			return (myEntityID == aEntity.GetID());
		}

		bool operator==(entt::entity aEntity)
		{
			return (myEntityID == aEntity);
		}

		entt::entity GetID()
		{
			return myEntityID;
		}




		bool IsNull();

	private:
		entt::entity myEntityID = entt::null;
		Scene* myScene;
	};


}