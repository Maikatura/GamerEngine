#include "GraphicsEngine.pch.h"
#include "GraphicsEngine.h"
#include <Framework/DX11.h>
#include <Scene/Scene.h>
#include <AssetHandlers/ModelAssetHandler.h>
#include "Audio/AudioManager.h"
#include "Input/Input.h"
#include "Render/Renderer.h"
#include "Managers/DropManager.h"
#include <Managers/CommandManager.h>
#include <Render/ForwardRenderer.h>
#include "Profiler/Profiler.h"
#include "Render/DeferredRenderer.h"
#include "Render/ShadowRenderer.h"
#include "Render/PostProcessRenderer.h"
#include "Scene/SceneManager.h"
#include "Font/Font.h"

GraphicsEngine* GraphicsEngine::Get()
{
	return myInstance;
}

GraphicsEngine::~GraphicsEngine()
{
	StopUpdateThread();

	TextureAssetHandler::Clear();
	RevokeDragDrop(myWindowHandle);
	OleUninitialize();
}

bool GraphicsEngine::Initialize(unsigned someX, unsigned someY,
	unsigned someWidth, unsigned someHeight,
	bool enableDeviceDebug, const std::wstring& aName, bool aBoolToUseEditor, bool isVRMode)
{
	OleInitialize(nullptr);

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

	myWindowSize.cx = static_cast<long>(someWidth);
	myWindowSize.cy = static_cast<long>(someHeight);

	myDropManager = MakeRef<DropManager>();
	RegisterDragDrop(myWindowHandle, myDropManager.get());


	if (!DX11::Get().Init(myWindowHandle, enableDeviceDebug, isVRMode))
	{
		GE_LOG_ERROR("Failed to init DirectX");
		return false;
	}

	Engine::Font font("resources\\fonts\\OpenSans.ttf");

	myForwardRenderer = MakeRef<ForwardRenderer>();
	myDeferredRenderer = MakeRef<DeferredRenderer>();
	myShadowRenderer = MakeRef<ShadowRenderer>();
	myPostProcessRenderer = MakeRef<PostProcessRenderer>();
	myGBuffer = MakeRef<GBuffer>();

	Time::Update();
	Input::Init();
	AudioManager::Init();
	SceneManager::Get().Initialize();
	
	if (!myForwardRenderer->Initialize())
	{
		GE_LOG_ERROR("Failed to init Forward Renderer");
		return false;
	}

	if (!myDeferredRenderer->Initialize())
	{
		return false;
	}

	if (!myShadowRenderer->Initialize())
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

	

	if (!myUseEditor)
	{
		gCPUProfiler.Initialize(1, 512);
	}


	return true;
}



LRESULT CALLBACK GraphicsEngine::WinProc(_In_ HWND hWnd, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam)
{

	Input::UpdateEvents(uMsg, wParam, lParam);

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

	

	switch (uMsg)
	{
		case WM_CREATE:
		{
			const CREATESTRUCT* createdStruct = reinterpret_cast<CREATESTRUCT*>(lParam);
			graphicsEnginePtr = static_cast<GraphicsEngine*>(createdStruct->lpCreateParams);
			break;
		}

		case WM_SIZE:
		{
			if (DX11::Get().GetDevice() != NULL)
			{
				Get()->SetWindowSize((UINT)LOWORD(lParam), (UINT)HIWORD(lParam));

				switch (wParam)
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
				default: break;
				}
			}
			break;
		}

		case WM_EXITSIZEMOVE:
		{
			if (DX11::Get().GetDevice() != NULL)
			{
				graphicsEnginePtr->SetUpdateBuffers(true);
			}
			break;
		}

		case WM_SYSCOMMAND:
		{
			if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
			{
				return 0;
			}
			break;
		}



		case WM_DESTROY:
		{
			PostQuitMessage(0);
			break;
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
	if (myIsMinimized) return;

	if (!myUseEditor)
	{
		PROFILE_FRAME();
	}

	if(myWantToResizeBuffers)
	{
		if(DX11::Get().GetSwapChain())
		{
			if (SceneManager::Get().GetStatus() == SceneStatus::Complete)
			{
				myGBuffer->Release();
				DX11::Get().Resize();
				const auto scene = SceneManager::Get().GetScene();
				scene->Resize({ static_cast<unsigned int>(Get()->GetWindowSize().cx), static_cast<unsigned int>(Get()->GetWindowSize().cy) });
				myGBuffer->CreateGBuffer();
				myPostProcessRenderer->ReInitialize();

			}


		}
		myWantToResizeBuffers = false;
	}


	//Vector4f clearColor = Renderer::GetClearColor();
	//DX11::Get().BeginFrame({ clearColor.x, clearColor.y, clearColor.z, clearColor.w });
	//
	ResetStates();

	Vector4f clearColor = Renderer::GetClearColor();
	clearColor.z = 1.0f;
	clearColor.w = 1.0f;

	const auto renderTarget = DX11::Get().GetRenderTargetView();
	DX11::Get().GetContext()->OMSetRenderTargets(1, &renderTarget, DX11::Get().GetDepthStencilView()->myDSV.Get());
	constexpr float clearDepth = 1.0f;
	constexpr UINT8 clearStencil = 0;

	DX11::Get().GetContext()->ClearDepthStencilView(DX11::Get().GetDepthStencilView()->myDSV.Get(), D3D11_CLEAR_DEPTH, clearDepth, clearStencil);
	DX11::Get().GetContext()->ClearRenderTargetView(DX11::Get().GetRenderTargetView(), &clearColor.x);
}

void GraphicsEngine::OnFrameUpdate(bool aShouldRunLoop)
{
	/*while (myIsRunning)
	{*/
	Time::Update();
	Input::Update();

	if (myIsMinimized) return;
	
	if (!SceneManager::Get().IsReady())
	{
		return;
	}


	if (!aShouldRunLoop && myIsPaused)
	{
		if (Input::IsKeyDown(VK_CONTROL) && Input::IsKeyPressed('Z'))
		{
			CommandManager::Undo();
		}

		if (Input::IsKeyDown(VK_CONTROL) && Input::IsKeyPressed('Y'))
		{
			CommandManager::Redo();
		}
	}


#if _DEBUG
	if (Input::IsKeyPressed(VK_F6))
	{
		unsigned int currentRenderMode = static_cast<unsigned int>(GraphicsEngine::Get()->GetRenderMode());
		currentRenderMode++;
		if (currentRenderMode == static_cast<unsigned char>(RenderMode::COUNT))
		{
			currentRenderMode = 0;
		}

		std::cout << "Render Mode: " << currentRenderMode << "\n";

		GraphicsEngine::Get()->SetRenderMode(static_cast<RenderMode>(currentRenderMode));
	}

	if (Input::IsKeyPressed(VK_F7))
	{
		myRenderPass++;

		if (GBuffer::GBufferTexture::EGBufferTexture_Count + 1 == myRenderPass)
		{
			myRenderPass = 0;
		}
	}
#endif

	{
		PROFILE_CPU_SCOPE("Update Loop");

		if (SceneManager::Get().GetScene())
		{
			SceneManager::Get().Update();
		}

	}

//#ifdef _Distribution
//	std::string fps = "FPS: " + std::to_string(Time::GetFPS());
//	std::cout << fps.c_str() << std::endl;
//#endif

	//}
}

void GraphicsEngine::RenderScene(const VREye anEye) const
{
	if (!SceneManager::Get().IsReady())
	{
		return;
	}

	const Ref<Scene> scene = SceneManager::Get().GetScene();

	if (!scene)
	{
		return;
	}

	if (SceneManager::Get().GetStatus() != SceneStatus::Complete)
	{
		return;
	}

	if (!Renderer::GetCamera())
	{
		return;
	}

	const Matrix4x4f projection = Renderer::GetCamera()->GetHMDMatrixProjectionEye(anEye);
	const Matrix4x4f view = Renderer::GetCamera()->GetCurrentViewProjectionMatrix(anEye);


	const std::vector<Light*> someLightList = scene->GetLights();

	const Ref<DirectionalLight>& directionalLight = scene->GetDirLight();
	const Ref<EnvironmentLight>& environmentLight = scene->GetEnvLight();
	const std::vector<RenderBuffer>& modelList = Renderer::GetModels();
	std::vector<RenderBuffer2D>& spriteList = Renderer::GetSprites();

	const float deltaTime = Time::GetDeltaTime();

	// TODO : CLEAN THIS MESS UP

	bool renderSSAO = true;
	if (Input::IsKeyDown('P'))
	{
		renderSSAO = false;
	}


	//RendererBase::SetDepthStencilState(DepthStencilState::Disabled);
	//RendererBase::SetBlendState(BlendState::None);
	

	
	{
		PROFILE_CPU_SCOPE("Render Shadows");
		myShadowRenderer->ClearResources();
	
		//myShadowRenderer->Render(environmentLight.get(), modelList);
		//myShadowRenderer->Render(directionalLight.get(), modelList);
	
		for (auto& light : someLightList)
		{
			myShadowRenderer->Render(light, modelList);
		}
	
	}
	

	auto depth = DX11::Get().GetDepthStencilView()->mySRV;
	DX11::Get().GetContext()->PSSetShaderResources(120, 1, &depth);

	RendererBase::SetDepthStencilState(DepthStencilState::ReadWrite);
	RendererBase::SetBlendState(BlendState::None);


	{
		PROFILE_CPU_SCOPE("Generate GBuffer");
		myGBuffer->ClearResource(0);
		myGBuffer->SetAsTarget();
		myDeferredRenderer->GenerateGBuffer(view, projection, modelList, deltaTime, 0, anEye);
		myGBuffer->ClearTarget();
		myGBuffer->SetAsResource(0);
	}

	{
		PROFILE_CPU_SCOPE("Render SSAO");
		myPostProcessRenderer->ClearTargets();
		if (renderSSAO == true) myPostProcessRenderer->Render(PostProcessRenderer::PP_SSAO, view, projection);
	}

	{
		PROFILE_CPU_SCOPE("Render With Deferred Renderer");
		RendererBase::SetBlendState(BlendState::Alpha);
		myDeferredRenderer->Render(view, projection, directionalLight, environmentLight, someLightList, deltaTime, 0, anEye);
		myDeferredRenderer->ClearTarget();
		myGBuffer->ClearResource(0);
		myGBuffer->ClearTarget();
	}

	{
		PROFILE_CPU_SCOPE("Render SSAO");
		myPostProcessRenderer->ClearTargets();
		if (renderSSAO == true) myPostProcessRenderer->Render(PostProcessRenderer::PP_SSAO, view, projection);
	}

	{
		DX11::Get().ResetRenderTarget(myUseEditor, true);
		PROFILE_CPU_SCOPE("Render With Forward Renderer (Models)");
		RendererBase::SetDepthStencilState(DepthStencilState::ReadWrite);
		RendererBase::SetBlendState(BlendState::None);
		myForwardRenderer->Render(view, projection, modelList, directionalLight, environmentLight, someLightList, anEye);
	}
}

void GraphicsEngine::OnFrameRender() const
{
	if (myWantToResizeBuffers) return;
	if (myIsMinimized) return;


	if (!SceneManager::Get().IsReady())
	{
		return;
	}

	const auto scene = SceneManager::Get().GetScene();

	if (!scene)
	{
		return;
	}

	if (!scene->IsReady())
	{
		return;
	}

	{
		PROFILE_CPU_SCOPE("Scene Render Setup");
		scene->OnRender();
	}


#if ENABLE_VR

	if (!myIsPaused)
	{




		DX11::Get().GetContext()->RSSetState(DX11::Get().GetFrontCulling());
		{
			PROFILE_CPU_SCOPE("Render Left Eye (VR)");
			DX11::Get().GetRightEyeView()->SetRenderTarget(DX11::Get().GetContext(), DX11::Get().GetDepthStencilView());
			//Clear the render to texture background to blue so we can differentiate it from the rest of the normal scene.

				// Clear the render to texture.
			DX11::Get().GetRightEyeView()->ClearRenderTarget(DX11::Get().GetContext(), DX11::Get().GetDepthStencilView(), 0.0f, 0.0f, 1.0f, 1.0f);

			// Render the scene now and it will draw to the render to texture instead of the back buffer.
			RenderScene(VREye::Left);
		}

		{
			PROFILE_CPU_SCOPE("Render Right Eye (VR)");
			
			// Set the render target to be the render to texture.
			DX11::Get().GetRightEyeView()->SetRenderTarget(DX11::Get().GetContext(), DX11::Get().GetDepthStencilView());
			//Clear the render to texture background to blue so we can differentiate it from the rest of the normal scene.

				// Clear the render to texture.
			DX11::Get().GetRightEyeView()->ClearRenderTarget(DX11::Get().GetContext(), DX11::Get().GetDepthStencilView(), 0.0f, 0.0f, 1.0f, 1.0f);

			// Render the scene now and it will draw to the render to texture instead of the back buffer.
			RenderScene(VREye::Right);
		}
	}

#endif
	//// Reset the render target back to the original back buffer and not the render to texture anymore.



	{
		PROFILE_CPU_SCOPE("View Render Setup");
		if (myUseEditor)
		{
			//DX11::Get().TurnZBufferOff();
			DX11::Get().GetScreenView()->SetRenderTarget(DX11::Get().GetContext(), DX11::Get().GetDepthStencilView()->myDSV.Get());
			DX11::Get().GetScreenView()->ClearRenderTarget(DX11::Get().GetContext(), nullptr, 0.5f, 0.5f, 0.5f, 1.0f);
		}
		else
		{
			//DX11::Get().TurnZBufferOff();
			//DX11::Get().GetContext()->RSSetState(DX11::Get().myBackCulling);
			

		}


	}

	{
		PROFILE_CPU_SCOPE("Render Screen");
		RenderScene(VREye::None);
	}

	
	/*DX11::Get().GetContext()->GSSetShader(nullptr, nullptr, 0);

	Renderer::Clear();*/


	{
		PROFILE_CPU_SCOPE("Clean Scene");
		scene->Clean();
		//LineRenderer::Get().Clear();
	}


}

void GraphicsEngine::StartUpdateThread()
{

	/*myUpdateThread = MakeRef<std::thread>([&]()
		{
		});*/

	myIsRunning = true;

}

void GraphicsEngine::StopUpdateThread()
{
}

void GraphicsEngine::EndFrame() const
{
	if (myIsMinimized) return;


	//{
	//	PROFILE_CPU_SCOPE("Final Render Call");
	//	DX11::Get().ResetRenderTarget(myUseEditor);
	//	myDeferredRenderer->RenderLate();
	//}

	{
		PROFILE_CPU_SCOPE("End Of Frame");
		DX11::Get().EndFrame();
	}


	//myGBuffer->Clear();

	
	//myDropManager->ClearPaths();

	if (SceneManager::Get().GetStatus() == SceneStatus::NeedSwap)
	{
		SceneManager::Get().SwapScene();
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

void GraphicsEngine::SetWinProc(const std::function<bool(HWND, UINT, WPARAM, LPARAM)>& aFunction)
{
	myWinProcFunction = aFunction;
}

void GraphicsEngine::ResetStates()
{
	PROFILE_CPU_SCOPE("Reset States");
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

std::vector<std::string> GraphicsEngine::GetDropPath() const
{
	return myDropManager->GetDropPaths();
}

bool GraphicsEngine::GetEngineUpdateRuntime() const
{
	return myUpdateCondition;
}

void GraphicsEngine::SetEngineUpdateRuntime(bool aCondition)
{
	myUpdateCondition = aCondition;
}

bool GraphicsEngine::GetPauseState() const
{
	return myIsPaused;
}

void GraphicsEngine::SetPauseState(bool aCondition)
{
	myIsPaused = aCondition;
}

bool GraphicsEngine::GetEditorMode() const
{
	return myUseEditor;
}

bool GraphicsEngine::GetEngineRunning() const
{
	return myIsRunning;
}

void GraphicsEngine::SetEngineRunning(bool aCondition)
{
	myIsRunning = aCondition;
}

int GraphicsEngine::GetRenderPass() const
{
	return myRenderPass;
}

Vector2ui GraphicsEngine::GetEditorWindowSize() const
{
	return myEditorWindowSize;
}

void GraphicsEngine::SetEditorWindowSize(Vector2ui aEditorWindowSize)
{
	myEditorWindowSize = aEditorWindowSize;

	myPostProcessRenderer->ReInitialize();
}

