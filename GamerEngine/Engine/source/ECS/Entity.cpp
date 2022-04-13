#include "pch.h"
#include "Entity.h"

namespace GamerEngine
{
	Entity::Entity(entt::entity aEntityID) : myEntityID(aEntityID)
	{
	}

	Entity::~Entity()
	{
		delete this;
	}
}
