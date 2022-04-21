#include "pch.h"
#include "Entity.h"
#include "ECS/Scene.h"

namespace GamerEngine
{
	Entity::Entity() : myEntityID(entt::null), myScene(nullptr)
	{
	}

	Entity::Entity(entt::entity aEntityID, Scene* aScene) : myEntityID(aEntityID), myScene(aScene)
	{
	}
}
