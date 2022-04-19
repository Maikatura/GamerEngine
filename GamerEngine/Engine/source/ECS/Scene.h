#pragma once
#include <entt/entt.hpp>

namespace GamerEngine
{

	class Entity;

	class Scene
	{

		friend class Entity;
	public:
		Scene();
		~Scene();

		Entity* CreateEntity();

		

	private:
		entt::registry myRegistry;
	};
}
