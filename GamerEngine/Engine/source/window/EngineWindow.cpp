#include "pch.h"

#include "EngineWindow.h"
#include "elems/input.h"
#include "ui/ContentBrowserPanel.h"
#include "ui/InspectorPanel.h"
#include "ui/NavigationPanel.h"
#include "ui/PropertyPanel.h"
#include "ui/HierarchyPanel.h"
#include "ui/SceneView.h"
#include "utils/EngineContext.h"


namespace GamerEngine
{
	
	GLWindow::GLWindow() : mIsRunning(true), mWindow(nullptr)
	{
		mUICtx = std::make_unique<UIContext>();
		mRenderCtx = std::make_unique<OpenGL_Context>();
		myInstacneWinodw = this;
	}

	GLWindow::~GLWindow()
	{
		mUICtx->end();

		mRenderCtx->end();
	}

	GLWindow* GLWindow::GetInstance()
	{
		return myInstacneWinodw;
	}

	bool GLWindow::Init(int width, int height, const std::string& title)
	{
		Width = width;
		Height = height;
		Title = title;

		mRenderCtx->Init(this);
		mUICtx->Init(this);
		myContext = std::make_unique<EngineContext>();
		return mIsRunning;
	}

	void GLWindow::OnResize(int width, int height)
	{
		Width = width;
		Height = height;

		myContext->GetSceneView()->Resize(Width, Height);
		Render();
	}

	void GLWindow::SetNativeWindow(void* window)
	{
		mWindow = (GLFWwindow*)window;
	}

	void GLWindow::SetUpContext()
	{
		/*auto context = myContext.get();
		context->mySceneView = mSceneView.get();*/
		//context->myInspector = myInspectorPanel.get();
	}

	void GLWindow::OnScroll(double delta)
	{
		//mSceneView->OnMouseWheel(-delta);
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

	bool GLWindow::IsRunning()
	{
		return mIsRunning; 
	}

	void GLWindow::Render()
	{
		mRenderCtx->PreRender();
		mUICtx->PreRender();
		
		// UI
		myContext->Render();
		// UI END

		mUICtx->PostRender();
		mRenderCtx->PostRender();

		HandleInput();
	}

	void GLWindow::HandleInput()
	{

		float cameraSpeed = 0.5f;

		if (glfwGetKey(mWindow, GLFW_KEY_W) == GLFW_PRESS)
		{
			myContext->GetSceneView()->SetCameraPos({0.0f, 0.0f, cameraSpeed });
		}

		if (glfwGetKey(mWindow, GLFW_KEY_S) == GLFW_PRESS)
		{
			myContext->GetSceneView()->SetCameraPos({ 0.0f, 0.0f, -cameraSpeed });
		}

		if (glfwGetKey(mWindow, GLFW_KEY_A) == GLFW_PRESS)
		{
			myContext->GetSceneView()->SetCameraPos({ cameraSpeed, 0.0f, 0.0f });
		}

		if (glfwGetKey(mWindow, GLFW_KEY_D) == GLFW_PRESS)
		{
			myContext->GetSceneView()->SetCameraPos({ -cameraSpeed, 0.0f, 0.0f });
		}


		//mSceneView->ResetView();
		

		myContext->GetSceneView()->OnMouseMove(0, 0, Input::GetPressedButton(mWindow));
	}

	void* GLWindow::GetNativeWindow()
	{
		return mWindow; 
	}
}
