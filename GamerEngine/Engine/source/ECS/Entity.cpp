#include "pch.h"
#include "Entity.h"

namespace GamerEngine
{
	Entity::Entity(entt::entity aEntityID, Scene* aScene)
	{
		myEntityID = aEntityID;
		myScene = aScene;
	}
}
