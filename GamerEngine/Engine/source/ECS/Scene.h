#pragma once
#include <entt/entt.hpp>
#include "elems/camera.h"
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
		void RenderCamera(GamerEngine::Shader* shader);

		entt::registry* GetRegistry();

		static GamerEngine::Camera* GetCamera();

	private:
		entt::registry myRegistry;
		inline static std::unique_ptr<GamerEngine::Camera> myCamera;
	};
}
