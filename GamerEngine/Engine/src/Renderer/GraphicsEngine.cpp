#include "GraphicsEngine.pch.h"
#include "GraphicsEngine.h"
#include <Renderer/Framework/DX11.h>
#include <Renderer/Scene/Scene.h>
#include <Renderer/AssetHandlers/ModelAssetHandler.h>
#include "Audio/AudioManager.h"
#include "Input/Input.h"
#include "Render/Renderer.h"
#include "Managers/DropManager.h"
#include <Renderer/Managers/CommandManager.h>

#include <Renderer/Render/ForwardRenderer.h>
#include "Render/DeferredRenderer.h"
#include "Render/ShadowRenderer.h"
#include "Render/PostProcessRenderer.h"
#include "Scene/SceneManager.h"
#include "Utilites/ProfilerMacro.h"
#include "Utilites/VisualProfiler.h"


GraphicsEngine* GraphicsEngine::Get()
{
	return myInstance;
}

GraphicsEngine::~GraphicsEngine()
{
	StopUpdateThread();

	STOP_PROFILE();
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
	if(!myUseEditor)
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

	myForwardRenderer = std::make_shared<ForwardRenderer>();
	myDeferredRenderer = std::make_shared<DeferredRenderer>();
	myShadowRenderer = std::make_shared<ShadowRenderer>();
	myPostProcessRenderer = std::make_shared<PostProcessRenderer>();
	myGBuffer = std::make_shared<GBuffer>();

	Time::Update();
	Input::Init();
	AudioManager::Init();
	SceneManager::Initialize();


	if(!myForwardRenderer->Initialize())
	{
		return false;
	}

	if(!myDeferredRenderer->Initialize())
	{
		return false;
	}

	if(!myShadowRenderer->Initialize())
	{
		return false;
	}

	if(!myPostProcessRenderer->Initialize())
	{
		return false;
	}

	if(!myGBuffer->CreateGBuffer())
	{
		return false;
	}

	if(!RendererBase::Init())
	{
		return false;
	}


	START_PROFILE("Start of Program");

	return true;
}


LRESULT CALLBACK GraphicsEngine::WinProc(_In_ HWND hWnd, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
	// We want to be able to access the Graphics Engine instance from inside this function.
	static GraphicsEngine* graphicsEnginePtr = nullptr;

	if (graphicsEnginePtr)
	{
		if (graphicsEnginePtr->myWinProcFunction)
		{
			if (graphicsEnginePtr->myWinProcFunction(hWnd, uMsg, wParam, lParam))
			{
				return true;
			}
		}
	}

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
			graphicsEnginePtr->SetEngineRunning(false);
			std::cout << "Test\n";
			break;
		}

		case WM_CLOSE:
		{
			graphicsEnginePtr->SetEngineRunning(false);
			std::cout << "Dropped something\n";
			break;
		}

	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void GraphicsEngine::BeginFrame()
{
	if(myIsMinimized) return;



	if(myWantToResizeBuffers)
	{
		if(DX11::SwapChain)
		{
			if (SceneManager::GetStatus() == SceneStatus::Complete)
			{
				auto scene = SceneManager::GetScene();
				myGBuffer->Release();
				DX11::Resize();
				scene->Resize({ static_cast<unsigned int>(Get()->GetWindowSize().cx), static_cast<unsigned int>(Get()->GetWindowSize().cy) });
				myGBuffer->CreateGBuffer();
				myPostProcessRenderer->ReInitialize();
				
			}

			
		}
		myWantToResizeBuffers = false;
	}

	Vector4f clearColor = Renderer::GetClearColor();
	DX11::BeginFrame({ clearColor.x, clearColor.y, clearColor.z, clearColor.w });

	ResetStates();
}

void GraphicsEngine::OnFrameUpdate(bool aShouldRunLoop)
{
	/*while (myIsRunning)
	{*/
		Time::Update();
		Input::Update();

		if(myIsMinimized) return;
		if(SceneManager::GetStatus() == SceneStatus::Complete)
		{
			auto scene = SceneManager::GetScene();
			if(!scene) return;



			if(!aShouldRunLoop && myIsPaused)
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

			scene->OnUpdate((aShouldRunLoop && !myIsPaused), SceneManager::GetStatus() == SceneStatus::Complete);
		}

		
		
	//}
}

void GraphicsEngine::OnFrameRender()
{
	if(myIsMinimized) return;




	bool renderSSAO = true;
	if(Input::IsKeyDown('P'))
	{
		renderSSAO = false;
	}

	static bool sssaoOn = true;

	if (Input::IsKeyPressed('P'))
	{
		sssaoOn = !sssaoOn;
	}


	auto scene = SceneManager::GetScene();

	if(!scene)
	{
		return;
	}

	scene->OnRender();

	std::vector<Light*>& someLightList = scene->GetLights();

	const std::shared_ptr<DirectionalLight>& directionalLight = scene->GetDirLight();
	const std::shared_ptr<EnvironmentLight>& environmentLight = scene->GetEnvLight();
	const std::vector<RenderBuffer>& modelList = Renderer::GetModels();
	std::vector<RenderBuffer2D>& spriteList = Renderer::GetSprites();

	float deltaTime = Time::GetDeltaTime();

	// TODO : CLEAN THIS MESS UP

	if(GetRenderModeInt() != 9)
	{
		{
			PROFILE_SCOPE("Render Shadows");
			myShadowRenderer->ClearResources();
			
			/*myShadowRenderer->Render(environmentLight.get(), modelList);
			myShadowRenderer->Render(directionalLight.get(), modelList);*/

			for (auto& light : someLightList)
			{
				myShadowRenderer->Render(light, modelList);
			}
			//myShadowRenderer->ClearResources();
		}
	}
	myShadowRenderer->ClearTarget();

	RendererBase::SetDepthStencilState(DepthStencilState::ReadWrite);
	RendererBase::SetBlendState(BlendState::None);

	myGBuffer->ClearResource(0);
	myGBuffer->SetAsTarget();

	{
		PROFILE_SCOPE("Generate GBuffer");
		myDeferredRenderer->GenerateGBuffer(modelList, deltaTime, 0);
	}

	myGBuffer->ClearTarget();
	myGBuffer->SetAsResource(0);
	

	{
		PROFILE_SCOPE("Render With Deferred Renderer");
		RendererBase::SetBlendState(BlendState::Alpha);
		myDeferredRenderer->Render(directionalLight, environmentLight, someLightList, deltaTime, 0);
		//myDeferredRenderer->ClearTarget();
	}
	myGBuffer->ClearTarget();

	{
		PROFILE_SCOPE("Render SSAO");
		if(sssaoOn)
		{
			myPostProcessRenderer->Render(PostProcessRenderer::PP_SSAO);
		}
		else
		{
			myPostProcessRenderer->ClearTargets();
		}
	}

	if(GetRenderModeInt() != 9)
	{
		{
			PROFILE_SCOPE("Render With Forward Renderer (Models)");
			RendererBase::SetDepthStencilState(DepthStencilState::ReadWrite);
			RendererBase::SetBlendState(BlendState::None);

			//myForwardRenderer->Render(modelList, directionalLight, environmentLight, someLightList);
		}

		{
			PROFILE_SCOPE("Render With Forward Renderer (Sprites)");
			RendererBase::SetDepthStencilState(DepthStencilState::Disabled);
			myForwardRenderer->RenderSprites(spriteList, directionalLight, environmentLight);
		}

		{
			PROFILE_SCOPE("Render Bloom");
			myPostProcessRenderer->Render(PostProcessRenderer::PP_BLOOM);
		}

		{
			PROFILE_SCOPE("Render Tonemap");
			myPostProcessRenderer->Render(PostProcessRenderer::PP_TONEMAP);
		}

		PROFILE_SCOPE("Final Render Call");
		myDeferredRenderer->RenderLate();
		RendererBase::SetBlendState(BlendState::Alpha);
	}


	DX11::Context->GSSetShader(nullptr, nullptr, 0);

	Renderer::Clear();

	scene->Clean();
}

void GraphicsEngine::StartUpdateThread()
{
	myIsRunning = true;
	myUpdateThread.reset(new std::thread([&]()
	{
		OnFrameUpdate(GetEngineUpdateRuntime());
	}));
}

void GraphicsEngine::StopUpdateThread()
{
	if (myUpdateThread)
	{
		myIsRunning = false;

		while (!myUpdateThread->joinable())
		{
			
		}

		myUpdateThread->join();
	}
}

void GraphicsEngine::EndFrame()
{
	if(myIsMinimized) return;


	DX11::Context->GSSetShader(nullptr, nullptr, 0);

	DX11::EndFrame();

	myGBuffer->Clear();
	myDropManager->ClearPaths();

	if(SceneManager::GetStatus() == SceneStatus::NeedSwap)
	{
		SceneManager::SwapScene();
	}
}

void GraphicsEngine::SetMinimized(bool aMinimizedState)
{
	myIsMinimized = aMinimizedState;
}

void GraphicsEngine::SetUpdateBuffers(bool aUpdate)
{
	myWantToResizeBuffers = aUpdate;
}

void GraphicsEngine::SetWinProc(std::function<bool(HWND, UINT, WPARAM, LPARAM)> aFunction)
{
	myWinProcFunction = aFunction;
}

void GraphicsEngine::ResetStates() const
{
	RendererBase::SetBlendState(BlendState::None);
	RendererBase::SetDepthStencilState(DepthStencilState::ReadWrite);
	RendererBase::SetSamplerState(0u, SamplerState::Default);
	RendererBase::SetSamplerState(1u, SamplerState::PointClamp);
	RendererBase::SetSamplerState(2u, SamplerState::Wrap);
	RendererBase::SetSamplerState(3u, SamplerState::PointWrap);
	RendererBase::SetSamplerState(4u, SamplerState::LinearClamp);
	RendererBase::SetSamplerState(5u, SamplerState::LinearWrap);
	RendererBase::SetSamplerState(6u, SamplerState::ComparisonLinearClamp);
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

bool GraphicsEngine::GetPauseState()
{
	return myIsPaused;
}

void GraphicsEngine::SetPauseState(bool aCondition)
{
	myIsPaused = aCondition;
}

bool GraphicsEngine::GetEngineRunning()
{
	return myIsRunning;
}

void GraphicsEngine::SetEngineRunning(bool aCondition)
{
	myIsRunning = aCondition;
}

