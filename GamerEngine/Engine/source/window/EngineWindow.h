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
#include "ui/SceneView.h"
#include "ui/ContentBrowserPanel.h"
#include "ui/InspectorPanel.h"
#include "ui/NavigationPanel.h"
#include "ui/PropertyPanel.h"
#include "ui/HierarchyPanel.h"
#include "utils/EngineContext.h"

using namespace GamerEngine;

namespace GamerEngine
{
	class GLWindow : public IWindow
	{
	public:

		GLWindow();

		~GLWindow();

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

		GLFWwindow* mWindow;

		// Render contexts
		std::unique_ptr<UIContext> mUICtx;
		std::unique_ptr<OpenGL_Context> mRenderCtx;

		// UI components
		std::unique_ptr<NavigationPanel> myNavPanel;
		std::unique_ptr<PropertyPanel> mPropertyPanel;
		std::unique_ptr<ContentBrowserPanel> myContentBrowserPanel;
		std::unique_ptr<InspectorPanel> myInspectorPanel;
		std::unique_ptr<HierarchyPanel> myHierarchyPanel;

		std::unique_ptr<SceneView> mSceneView;

		std::unique_ptr<EngineContext> myContext;

		bool mIsRunning;

	};
}


