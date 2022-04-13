#pragma once
#include <entt/entt.hpp>
#include "Entity.h"

namespace GamerEngine
{
	class Scene
	{
	public:
		Scene();
		~Scene();

		Entity* CreateEntity();

		

	private:

		
		entt::registry myRegistry;
	};
}
