#pragma once

#include "elems/mesh.h"
#include "elems/camera.h"
#include "elems/light.h"
#include "shader/shader_util.h"

#include "render/ui_context.h"
#include "render/opengl_context.h"
#include "render/opengl_buffer_manager.h"
#include "ui/GUIHandler.h"
#include "window/window.h"



using namespace GamerEngine;

namespace GamerEngine
{
	class SceneView;
	class EngineContext;
	
	class GLWindow : public IWindow
	{
	public:

		GLWindow();
		~GLWindow();

		static GLWindow* GetInstance();

		bool Init(int width, int height, const std::string& title);
		void Render();
		void HandleInput();
		void* GetNativeWindow() override;
		void SetNativeWindow(void* window) override;
		void SetUpContext();

		void OnScroll(double delta) override;
		void OnKey(int key, int scancode, int action, int mods) override;
		void OnResize(int width, int height) override;
		void OnClose() override;

		bool IsRunning();


	private:

		inline static GLWindow* myInstacneWinodw;
		GLFWwindow* mWindow;

		// Render contexts
		std::unique_ptr<UIContext> mUICtx;
		std::unique_ptr<OpenGL_Context> mRenderCtx;

		// UI components
		
		std::unique_ptr<EngineContext> myContext;
		std::unique_ptr<SceneView> mSceneView;


		bool mIsRunning;

	};
}


