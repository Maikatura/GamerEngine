#include "GraphicsEngine.pch.h"
#include "GraphicsEngine.h"
#include <Framework/DX11.h>
#include <Render/ForwardRenderer.h>
#include <Scene/Scene.h>
#include <Render/ForwardRenderer.h>
#include <AssetHandlers/ModelAssetHandler.h>
#include <ImGui/imgui.h>

#include "Audio/AudioManager.h"


GraphicsEngine* GraphicsEngine::Get()
{
	return myInstance;
}

GraphicsEngine::~GraphicsEngine()
{
#if _DEBUG
#pragma warning( push )
#pragma warning( disable : 4996 )
		fclose(stdin);
		fclose(stdout);
		fclose(stderr);
#pragma warning( pop )
#endif
}

bool GraphicsEngine::Initialize(unsigned someX, unsigned someY,
	unsigned someWidth, unsigned someHeight,
	bool enableDeviceDebug, std::wstring aName)
{
	myInstance = this;

#if _DEBUG
#pragma warning( push )
#pragma warning( disable : 4996 )
	AllocConsole();
	freopen("CONIN$", "r", stdin);
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);

	setbuf(stdin, NULL);
	setbuf(stdout, NULL);
	setbuf(stderr, NULL);
#pragma warning( pop )
#endif

	// Initialize our window:
	WNDCLASS windowClass = {};
	windowClass.style = CS_VREDRAW | CS_HREDRAW | CS_OWNDC;
	windowClass.lpfnWndProc = GraphicsEngine::WinProc;
	windowClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
	windowClass.lpszClassName = L"TGP";
	RegisterClass(&windowClass);

	myWindowHandle = CreateWindow(
		L"TGP",
		aName.c_str(),
		WS_OVERLAPPEDWINDOW | WS_POPUP | WS_VISIBLE,
		someX,
		someY,
		someWidth,
		someHeight,
		nullptr, nullptr, nullptr,
		this
	);

	myWindowSize.cx = someWidth;
	myWindowSize.cy = someHeight;


	if(!DX11::Init(myWindowHandle, enableDeviceDebug))
	{
		return false;
	}

	myScene = std::make_shared<Scene>();
	myForwardRenderer = std::make_shared<ForwardRenderer>();
	myInputManager = std::make_shared<CommonUtilities::InputManager>();

	Time::Update();

	AudioManager::Init();


	if(!ModelAssetHandler::Initialize())
	{
		return false;
	}

	if(!myScene->Initialize())
	{
		return false;
	}

	if(!myForwardRenderer->Initialize())
	{
		return false;
	}

	std::wstring chestPath = L"Assets\\Models\\Particle_Chest.fbx";
	ModelAssetHandler::LoadModel(chestPath);
	std::shared_ptr<ModelInstance> chest = ModelAssetHandler::GetModelInstance(chestPath);
	chest->GetTransform().SetPosition({ -2.0f, 0.0f, 1.0f });
	chest->GetTransform().SetScale({ 0.02f, 0.02f, 0.02f });

	std::wstring goblinPath = L"Assets\\Models\\Gremlin.fbx";
	std::wstring goblinAnimPath = L"Assets\\Models\\gremlin@walk.fbx";
	ModelAssetHandler::LoadModel(goblinPath);
	std::shared_ptr<ModelInstance> goblin = ModelAssetHandler::GetModelInstance(goblinPath);
	goblin->GetTransform().SetPosition({ 2.5f, 0.0f, 1.0f });
	goblin->GetTransform().SetScale({ 0.1f, 0.1f, 0.1f });
	ModelAssetHandler::LoadAnimation(goblinPath, goblinAnimPath);
	goblin->PlayAnimation(goblinAnimPath);

	ModelAssetHandler::LoadModel(goblinPath);
	std::shared_ptr<ModelInstance> goblin2 = ModelAssetHandler::GetModelInstance(goblinPath);
	goblin2->GetTransform().SetPosition({ -5.5f, 0.0f, 1.0f });
	goblin2->GetTransform().SetScale({ 0.1f, 0.1f, 0.1f });
	ModelAssetHandler::LoadAnimation(goblinPath, goblinAnimPath);
	goblin2->PlayAnimation(goblinAnimPath);

	myScene->AddSceneObject(chest);
	myScene->AddSceneObject(goblin);
	myScene->AddSceneObject(goblin2);

	return true;
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK GraphicsEngine::WinProc(_In_ HWND hWnd, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
	// We want to be able to access the Graphics Engine instance from inside this function.
	static GraphicsEngine* graphicsEnginePtr = nullptr;

	if(ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

	Get()->GetInput()->UpdateEvents(uMsg, wParam, lParam);

	switch(uMsg)
	{
		case WM_SIZE:
		{
			if(DX11::Device != NULL)
			{
				Get()->SetWindowSize((UINT)LOWORD(lParam), (UINT)HIWORD(lParam));

				switch(wParam)
				{
					case SIZE_MAXIMIZED:
						Get()->SetUpdateBuffers(true);
						Get()->SetMinimized(false);
						break;

					case SIZE_MINIMIZED:
						Get()->SetMinimized(true);
						break;

					case SIZE_RESTORED:
						Get()->SetUpdateBuffers(true);
						Get()->SetMinimized(false);
						break;

					case SIZE_MAXSHOW:
						Get()->SetUpdateBuffers(true);
						Get()->SetMinimized(false);
						break;
				}
			}
		}

		case WM_EXITSIZEMOVE:
		{
			if(DX11::Device != NULL)
			{
				Get()->SetUpdateBuffers(true);
			}
		}

		case WM_SYSCOMMAND:
		{
			if((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
			{
				return 0;
			}
			break;
		}

		case WM_DESTROY:
		{
			PostQuitMessage(0);
		}

		case WM_DPICHANGED:
		{
			break;
		}
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void GraphicsEngine::BeginFrame()
{
	if (myIsMinimized) return;

	myInputManager->Update();
	Time::Update();

	if (myUpdateBuffers)
	{
		if(DX11::SwapChain)
		{
			myScene->GetCamera()->Resize({ static_cast<unsigned int>(Get()->GetWindowSize().cx), static_cast<unsigned int>(Get()->GetWindowSize().cy) });
			DX11::Resize();
		}
		myUpdateBuffers = false;
	}
	
	DX11::Context->OMSetRenderTargets(1, DX11::BackBuffer.GetAddressOf(), DX11::DepthBuffer.Get());
	DX11::BeginFrame({ myClearColor.x, myClearColor.y, myClearColor.z, myClearColor.w });
}

void GraphicsEngine::RenderFrame()
{
	if(myIsMinimized) return;

	if(myScene)
	{
		const std::shared_ptr<Camera>& camera = myScene->GetCamera();
		camera->Update();

		const std::vector<std::shared_ptr<ModelInstance>> modelsToRender = myScene->CullModels(camera);

		for(size_t i = 0; i < modelsToRender.size(); i++)
		{
			modelsToRender[i]->Update();
		}

		myForwardRenderer->Render(camera, modelsToRender);
	}
}

void GraphicsEngine::EndFrame()
{
	if(myIsMinimized) return;

	DX11::EndFrame();
}

void GraphicsEngine::SetMinimized(bool aMinimizedState)
{
	myIsMinimized = aMinimizedState;
}

void GraphicsEngine::SetUpdateBuffers(bool aUpdate)
{
	myUpdateBuffers = aUpdate;
}

void GraphicsEngine::SetColor(CommonUtilities::Vector4<float> aClearColor)
{
	myClearColor = aClearColor;
}

CommonUtilities::Vector4<float> GraphicsEngine::GetClearColor()
{
	return myClearColor;
}

std::shared_ptr<Scene> GraphicsEngine::GetScene()
{
	return myScene;
}

std::shared_ptr<CommonUtilities::InputManager> GraphicsEngine::GetInput()
{
	return myInputManager;
}

