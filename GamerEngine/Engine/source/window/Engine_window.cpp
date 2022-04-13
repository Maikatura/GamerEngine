#include "pch.h"

#include "engine_window.h"
#include "elems/input.h"

namespace nwindow
{
	
	GLWindow::GLWindow() : mIsRunning(true), mWindow(nullptr)
	{
		mUICtx = std::make_unique<UIContext>();
		mRenderCtx = std::make_unique<OpenGL_Context>();
	}

	GLWindow::~GLWindow()
	{
		mUICtx->end();

		mRenderCtx->end();
	}

	bool GLWindow::Init(int width, int height, const std::string& title)
	{
		Width = width;
		Height = height;
		Title = title;

		mRenderCtx->Init(this);

		mUICtx->Init(this);

		mSceneView = std::make_unique<SceneView>();

		mPropertyPanel = std::make_unique<Property_Panel>();
		myContentBrowserPanel = std::make_unique<ContentBrowserPanel>();
		myInspectorPanel = std::make_unique<InspectorPanel>();

		mPropertyPanel->set_mesh_load_callback(
			[this](std::string filepath) { mSceneView->load_mesh(filepath); });

		return mIsRunning;
	}

	void GLWindow::OnResize(int width, int height)
	{
		Width = width;
		Height = height;

		mSceneView->Resize(Width, Height);
		Render();
	}

	void GLWindow::OnScroll(double delta)
	{
		mSceneView->on_mouse_wheel(-delta);
	}

	void GLWindow::OnKey(int key, int scancode, int action, int mods)
	{
		if (action == GLFW_PRESS)
		{
		}
	}

	void GLWindow::OnClose()
	{
		mIsRunning = false;
	}

	void GLWindow::Render()
	{
		mRenderCtx->PreRender();
		mUICtx->PreRender();
		mSceneView->Render();

		// UI
		mPropertyPanel->OnImGuiRender(mSceneView.get());
		myContentBrowserPanel->OnImGuiRender();
		myInspectorPanel->OnImGuiRender(mSceneView.get());
		// UI END

		mUICtx->PostRender();
		mRenderCtx->PostRender();

		HandleInput();
	}

	void GLWindow::HandleInput()
	{
		if (glfwGetKey(mWindow, GLFW_KEY_W) == GLFW_PRESS)
		{
			mSceneView->on_mouse_wheel(-0.4f);
		}

		if (glfwGetKey(mWindow, GLFW_KEY_S) == GLFW_PRESS)
		{
			mSceneView->on_mouse_wheel(0.4f);
		}

		if (glfwGetKey(mWindow, GLFW_KEY_F) == GLFW_PRESS)
		{
			mSceneView->reset_view();
		}

		double x, y;
		glfwGetCursorPos(mWindow, &x, &y);

		mSceneView->on_mouse_move(x, y, Input::GetPressedButton(mWindow));
	}
}
