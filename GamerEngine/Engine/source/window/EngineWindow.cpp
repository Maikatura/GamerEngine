#include "pch.h"

#include "EngineWindow.h"
#include "elems/input.h"


namespace GamerEngine
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
		mPropertyPanel = std::make_unique<PropertyPanel>();
		myContentBrowserPanel = std::make_unique<ContentBrowserPanel>();
		myInspectorPanel = std::make_unique<InspectorPanel>();
		myNavPanel = std::make_unique<NavigationPanel>();
		myHierarchyPanel = std::make_unique<HierarchyPanel>();

		myContext = std::make_unique<EngineContext>();
		SetUpContext();

		mPropertyPanel->SetMeshLoadCallback(
			[this](std::string filepath) { mSceneView->LoadMesh(filepath); });

		return mIsRunning;
	}

	void GLWindow::OnResize(int width, int height)
	{
		Width = width;
		Height = height;

		mSceneView->Resize(Width, Height);
		Render();
	}

	void GLWindow::SetNativeWindow(void* window)
	{
		
		mWindow = (GLFWwindow*)window;
	}

	void GLWindow::SetUpContext()
	{
		auto context = myContext.get();
		context->mySceneView = mSceneView.get();
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

		mSceneView->Render();

		// UI
		myNavPanel->OnImGuiRender();
		mPropertyPanel->OnImGuiRender(mSceneView.get());
		myContentBrowserPanel->OnImGuiRender();
		myInspectorPanel->OnImGuiRender(myContext.get());
		myHierarchyPanel->OnImGuiRender(myContext.get());
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
			mSceneView->SetCameraPos({0.0f, 0.0f, cameraSpeed });
		}

		if (glfwGetKey(mWindow, GLFW_KEY_S) == GLFW_PRESS)
		{
			mSceneView->SetCameraPos({ 0.0f, 0.0f, -cameraSpeed });
		}

		if (glfwGetKey(mWindow, GLFW_KEY_A) == GLFW_PRESS)
		{
			mSceneView->SetCameraPos({ cameraSpeed, 0.0f, 0.0f });
		}

		if (glfwGetKey(mWindow, GLFW_KEY_D) == GLFW_PRESS)
		{
			mSceneView->SetCameraPos({ -cameraSpeed, 0.0f, 0.0f });
		}


		//mSceneView->ResetView();

		double x, y;
		glfwGetCursorPos(mWindow, &x, &y);

		mSceneView->OnMouseMove(x, y, Input::GetPressedButton(mWindow));
	}

	void* GLWindow::GetNativeWindow()
	{
		return mWindow; 
	}
}
