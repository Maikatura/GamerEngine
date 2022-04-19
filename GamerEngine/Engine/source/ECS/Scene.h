#pragma once
#include <entt/entt.hpp>

#include "shader/shader_util.h"

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


		void Render(GamerEngine::Shader* shader);
		

	private:
		entt::registry myRegistry;
	};
}
