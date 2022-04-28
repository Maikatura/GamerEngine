#pragma once
#include <AllocationTracker.h>

#include "elems/camera.h"
#include "elems/mesh.h"
#include "elems/light.h"
#include "shader/shader_util.h"
#include "render/opengl_buffer_manager.h"
#include "elems/input.h"
#include "Components/MeshComponent.h"
#include "ECS/Scene.h"

namespace GamerEngine
{
	class SceneView
	{
	public:
		SceneView();

		~SceneView();

		GamerEngine::Light* get_light() { return mLight.get(); }

		void Resize(int32_t width, int32_t height);

		void Render();

		
		void LoadMesh(const std::string& filepath);
		
		void SetCameraPos(glm::vec3 aPosition);

		void OnMouseMove(double x, double y, GamerEngine::EInputButton button);

		inline static Scene* GetScene();

		/*void OnMouseWheel(double delta);

		void ResetView();*/

	private:
		std::unique_ptr<GamerEngine::OpenGL_FrameBuffer> mFrameBuffer;
		std::unique_ptr<GamerEngine::Shader> myCameraShader;
		std::unique_ptr<GamerEngine::Shader> myModelShader;
		std::unique_ptr<GamerEngine::Light> mLight;

		inline static std::unique_ptr<Scene> myScene;

		glm::vec2 mSize;
	};
}

