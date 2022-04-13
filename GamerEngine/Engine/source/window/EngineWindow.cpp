#include "pch.h"

#include "EngineWindow.h"
#include "elems/input.h"

namespace nwindow
{
	bool GLWindow::init(int width, int height, const std::string& title)
	{
		Width = width;
		Height = height;
		Title = title;

		mRenderCtx->init(this);

		mUICtx->init(this);

		mSceneView = std::make_unique<SceneView>();

		mPropertyPanel = std::make_unique<Property_Panel>();
		myContentBrowserPanel = std::make_unique<ContentBrowserPanel>();
		myInspectorPanel = std::make_unique<InspectorPanel>();

		mPropertyPanel->set_mesh_load_callback(
			[this](std::string filepath) { mSceneView->load_mesh(filepath); });

		return mIsRunning;
	}

	GLWindow::~GLWindow()
	{
		mUICtx->end();

		mRenderCtx->end();
	}

	void GLWindow::OnResize(int width, int height)
	{
		Width = width;
		Height = height;

		mSceneView->Resize(Width, Height);
		render();
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

	void GLWindow::render()
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
