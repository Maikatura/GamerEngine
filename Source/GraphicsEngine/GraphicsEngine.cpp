#include "GraphicsEngine.pch.h"
#include "GraphicsEngine.h"
#include <Frameworks/Framework_DX11.h>
#include <Render/ForwardRenderer.h>
#include <Scene/Scene.h>
#include <Render/ForwardRenderer.h>
#include <Scene/ModelAssetHandler.h>

bool GraphicsEngine::Initialize(unsigned someX, unsigned someY,
	unsigned someWidth, unsigned someHeight,
	bool enableDeviceDebug)
{
	// Initialize our window:
	WNDCLASS windowClass = {};
	windowClass.style = CS_VREDRAW | CS_HREDRAW | CS_OWNDC;
	windowClass.lpfnWndProc = GraphicsEngine::WinProc;
	windowClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
	windowClass.lpszClassName = L"TGP";
	RegisterClass(&windowClass);

	myWindowHandle = CreateWindow(
		L"GamerEngine",
		L"GamerEngine",
		WS_OVERLAPPEDWINDOW | WS_POPUP | WS_VISIBLE,
		someX,
		someY,
		someWidth,
		someHeight,
		nullptr, nullptr, nullptr,
		this
	);

	if (!DX11::Init(myWindowHandle, true))
	{
		return false;
	}

	myScene = std::make_shared<Scene>();
	myAssetHandler = std::make_shared<ModelAssetHandler>();
	myForwardRenderer = std::make_shared<ForwardRenderer>();
	
	if (!myAssetHandler->Initialize()) 
	{
		return false;
	}
	
	if (!myScene->Initialize())
	{
		return false;
	}
	
	if (!myForwardRenderer->Initialize())
	{
		return false;
	}

	auto test = myAssetHandler->GetModel(L"Cube");
	test->GetTransform().SetPosition({ 0.0f, 0.0f, 5.0f, 1.0f });
	myScene->AddGameObject(test);

	return true;
}

LRESULT CALLBACK GraphicsEngine::WinProc(_In_ HWND hWnd, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
	// We want to be able to access the Graphics Engine instance from inside this function.
	static GraphicsEngine* graphicsEnginePtr = nullptr;

	if(uMsg == WM_DESTROY || uMsg == WM_CLOSE)
	{
		PostQuitMessage(0);
	}
	else if (uMsg == WM_CREATE)
	{
		const CREATESTRUCT* createdStruct = reinterpret_cast<CREATESTRUCT*>(lParam);
		graphicsEnginePtr = static_cast<GraphicsEngine*>(createdStruct->lpCreateParams);
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void GraphicsEngine::BeginFrame()
{
	DX11::BeginFrame({ 0.0f, 1.0f, 0.5f, 1.0f });
}

void GraphicsEngine::RenderFrame()
{
	if (myScene) 
	{
		const std::shared_ptr<Camera> camera = myScene->GetCamera();
		const std::vector<std::shared_ptr<Model>> modelsToRender = myScene->CullModels(camera);
		myForwardRenderer->Render(camera, modelsToRender);
	}
}

void GraphicsEngine::EndFrame()
{
	DX11::EndFrame();
}
