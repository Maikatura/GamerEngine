#pragma once
#include "entt/entt.hpp"

namespace GamerEngine
{
	class Entity
	{
	public:
		Entity(entt::entity aEntityID);
		~Entity();

	private:
		entt::entity myEntityID;
	};
}