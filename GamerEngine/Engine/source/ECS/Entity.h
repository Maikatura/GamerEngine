#pragma once
#include "entt/entt.hpp"

namespace GamerEngine
{
	class Entity
	{
	public:
		Entity(entt::entity aEntityID);
		~Entity();


		template<typename T>
		bool HasComponent()
		{
			// Check if EnTT has component
		}

	private:
		entt::entity myEntityID;
	};
}