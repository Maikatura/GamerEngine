#include "GraphicsEngine.pch.h"
#include "GraphicsEngine.h"
#include <Framework/DX11.h>
#include <Scene/Scene.h>
#include <AssetHandlers/ModelAssetHandler.h>
#include <ImGui/imgui.h>
#include "Audio/AudioManager.h"
#include "Input/Input.h"
#include "Render/Renderer.h"
#include "Managers/DropManager.h"
#include <Managers/CommandManager.h>

#include <Render/ForwardRenderer.h>
#include "Render/DeferredRenderer.h"
#include "Render/ShadowRenderer.h"
#include "Render/PostProcessRenderer.h"


GraphicsEngine* GraphicsEngine::Get()
{
	return myInstance;
}

GraphicsEngine::~GraphicsEngine()
{
	ModelAssetHandler::Clear();
	TextureAssetHandler::Clear();
	RevokeDragDrop(myWindowHandle);
	OleUninitialize();
}

bool GraphicsEngine::Initialize(unsigned someX, unsigned someY,
	unsigned someWidth, unsigned someHeight,
	bool enableDeviceDebug, std::wstring aName, bool aBoolToUseEditor)
{
	OleInitialize(NULL);

	myInstance = this;
	myUseEditor = aBoolToUseEditor;
	if (!myUseEditor)
	{
		myUpdateCondition = true;
	}

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

	myDropManager = std::make_shared<DropManager>();
	RegisterDragDrop(myWindowHandle, myDropManager.get());

	if(!DX11::Init(myWindowHandle, enableDeviceDebug))
	{
		return false;
	}

	myScene = std::make_shared<Scene>();
	myForwardRenderer = std::make_shared<ForwardRenderer>();
	myDeferredRenderer = std::make_shared<DeferredRenderer>();
	myShadowRenderer = std::make_shared<ShadowRenderer>();
	myPostProcessRenderer = std::make_shared<PostProcessRenderer>();
	myGBuffer = std::make_shared<GBuffer>();

	Time::Update();
	Input::Init();
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

	if (!myDeferredRenderer->Initialize())
	{
		return false;
	}

	if(!myShadowRenderer->Initialize())
	{
		return false;
	}

	if (!myPostProcessRenderer->Initialize())
	{
		return false;
	}

	if (!myGBuffer->CreateGBuffer())
	{
		return false;
	}

	if (!RendererBase::Init())
	{
		return false;
	}

	return true;
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK GraphicsEngine::WinProc(_In_ HWND hWnd, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
	// We want to be able to access the Graphics Engine instance from inside this function.
	static GraphicsEngine* graphicsEnginePtr = nullptr;

	if(ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

	Input::UpdateEvents(uMsg, wParam, lParam);

	switch(uMsg)
	{
		case WM_CREATE:
		{
			const CREATESTRUCT* createdStruct = reinterpret_cast<CREATESTRUCT*>(lParam);
			graphicsEnginePtr = static_cast<GraphicsEngine*>(createdStruct->lpCreateParams);
		}

		case WM_SIZE:
		{
			if(DX11::Device != NULL)
			{
				Get()->SetWindowSize((UINT)LOWORD(lParam), (UINT)HIWORD(lParam));

				switch(wParam)
				{
					case SIZE_MAXIMIZED:
						graphicsEnginePtr->SetUpdateBuffers(true);
						graphicsEnginePtr->SetMinimized(false);
						break;

					case SIZE_MINIMIZED:
						graphicsEnginePtr->SetMinimized(true);
						break;

					case SIZE_RESTORED:
						graphicsEnginePtr->SetUpdateBuffers(true);
						graphicsEnginePtr->SetMinimized(false);
						break;

					case SIZE_MAXSHOW:
						graphicsEnginePtr->SetUpdateBuffers(true);
						graphicsEnginePtr->SetMinimized(false);
						break;
				}
			}
		}

		case WM_EXITSIZEMOVE:
		{
			if(DX11::Device != NULL)
			{
				graphicsEnginePtr->SetUpdateBuffers(true);
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

		case WM_DROPFILES:
		{
			std::cout << "Dropped something\n";
			break;
		}

		case WM_QUIT:
		{
			std::cout << "Test\n";
			break;
		}

		case WM_CLOSE:
		{
			std::cout << "Dropped something\n";
			break;
		}

	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void GraphicsEngine::BeginFrame()
{
	

	if (myIsMinimized) return;

	if (myWantToResizeBuffers)
	{
		if(DX11::SwapChain)
		{
			myGBuffer->Release();
			myScene->Resize({ static_cast<unsigned int>(Get()->GetWindowSize().cx), static_cast<unsigned int>(Get()->GetWindowSize().cy) });
			DX11::Resize();
			myGBuffer->CreateGBuffer();
		}
		myWantToResizeBuffers = false;
	}


	Vector4f clearColor = Renderer::GetClearColor();
	DX11::BeginFrame({ clearColor.x, clearColor.y, clearColor.z, clearColor.w });
	
	ResetStates();
}

void GraphicsEngine::OnFrameUpdate(bool aShouldRunLoop)
{
	if(myIsMinimized) return;

	Time::Update();
	Input::Update();

	if(myScene)
	{
		if (!myUpdateCondition)
		{
			if(Input::IsKeyDown(VK_CONTROL) && Input::IsKeyPressed('Z'))
			{
				CommandManager::Undo();
			}

			if(Input::IsKeyDown(VK_CONTROL) && Input::IsKeyPressed('Y'))
			{
				CommandManager::Redo();
			}
		}

		myScene->OnUpdate(myUpdateCondition);
	}
}

void GraphicsEngine::OnFrameRender()
{
	if(myIsMinimized) return;

	if(myScene)
	{
		myScene->OnRender();

		std::vector<Light*> someLightList = Renderer::GetSomeLights();
		
		const std::shared_ptr<DirectionalLight>& directionalLight = myScene->GetDirLight();
		const std::shared_ptr<EnvironmentLight>& environmentLight = myScene->GetEnvLight();
		const std::vector<RenderBuffer>& modelList = Renderer::GetModels();
		std::vector<RenderBuffer2D>& spriteList = Renderer::GetSprites();

		float deltaTime = Time::GetDeltaTime();

		// TODO : CLEAN THIS MESS UP

		RendererBase::SetDepthStencilState(DepthStencilState::DSS_ReadWrite);
		RendererBase::SetBlendState(BlendState::NoBlend);
		myShadowRenderer->ClearResources();
		myShadowRenderer->Render(modelList);

		myGBuffer->ClearResource(0);
		myGBuffer->SetAsTarget();
		myDeferredRenderer->GenerateGBuffer(modelList, deltaTime, 0);

		myGBuffer->ClearTarget();
		myGBuffer->SetAsResource(0);

		
		RendererBase::SetDepthStencilState(DepthStencilState::DSS_Ignore);
		myDeferredRenderer->Render(directionalLight, environmentLight, someLightList, deltaTime, 0);

		RendererBase::SetDepthStencilState(DepthStencilState::DSS_ReadWrite);
		RendererBase::SetBlendState(BlendState::NoBlend);
		myForwardRenderer->Render(modelList, directionalLight, environmentLight, someLightList);

		RendererBase::SetDepthStencilState(DepthStencilState::DSS_Ignore);
		myForwardRenderer->RenderSprites(spriteList, directionalLight, environmentLight);
		
		//myPostProcessRenderer->Render(PostProcessRenderer::PP_BLOOM);

		RendererBase::SetBlendState(BlendState::AlphaBlend);
	}



	DX11::Context->GSSetShader(nullptr, nullptr, 0);

	Renderer::Clear();

	myScene->Clean();
}

void GraphicsEngine::EndFrame()
{
	if(myIsMinimized) return;

	
	//DX11::Context->GSSetShader(nullptr, nullptr, 0);

	DX11::EndFrame();

	myGBuffer->Clear();
	myDropManager->ClearPaths();
}

void GraphicsEngine::SetMinimized(bool aMinimizedState)
{
	myIsMinimized = aMinimizedState;
}

void GraphicsEngine::SetUpdateBuffers(bool aUpdate)
{
	myWantToResizeBuffers = aUpdate;
}

bool GraphicsEngine::UseEditor() const
{
	return myUseEditor;
}

void GraphicsEngine::ResetStates() const
{
	RendererBase::SetBlendState(BlendState::NoBlend);
	RendererBase::SetDepthStencilState(DepthStencilState::DSS_ReadWrite);
	RendererBase::SetSamplerState(SamplerState::SS_Default, 0);
	RendererBase::SetSamplerState(SamplerState::SS_PointClamp, 1);
}

std::shared_ptr<Scene> GraphicsEngine::GetScene()
{
	return myScene;
}

std::shared_ptr<CommonUtilities::InputManager> GraphicsEngine::GetInput()
{
	return myInputManager;
}

std::vector<std::string> GraphicsEngine::GetDropPath()
{
	return myDropManager->GetDropPaths();
}

bool GraphicsEngine::GetEngineUpdateRuntime()
{
	return myUpdateCondition;
}

void GraphicsEngine::SetEngineUpdateRuntime(bool aCondition)
{
	myUpdateCondition = aCondition;
}

