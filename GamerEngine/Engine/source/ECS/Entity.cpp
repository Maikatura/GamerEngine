#include "pch.h"
#include "Entity.h"
#include "ECS/Scene.h"

namespace GamerEngine
{
	Entity::Entity()
	{
	}

	Entity::Entity(entt::entity aEntityID, Scene* aScene)
	{
		myEntityID = aEntityID;
		myScene = aScene;
	}

	bool Entity::IsNull()
	{
		return myEntityID == entt::null;
	}

	
}
