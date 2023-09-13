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
	bool enableDeviceDebug, std::wstring aName, bool aBoolToUseEditor, bool isVRMode)
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

	
	if (!DX11::Init(myWindowHandle, enableDeviceDebug, isVRMode))
	{
		return false;
	}

	myForwardRenderer = std::make_shared<ForwardRenderer>();
	myDeferredRenderer = std::make_shared<DeferredRenderer>();
	myShadowRenderer = std::make_shared<ShadowRenderer>();
	myPostProcessRenderer = std::make_shared<PostProcessRenderer>();
	myGBuffer = std::make_shared<GBuffer>();

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

	Time::Update();
	Input::Init();
	AudioManager::Init();
	SceneManager::Initialize();

	START_PROFILE("Start of Program");

	return true;
}

Matrix4x4f ConvertSteamVRMatrixToMatrix4(const vr::HmdMatrix34_t& matPose)
{
	Matrix4x4f matrixObj;
	matrixObj(0) = matPose.m[0][0];
	matrixObj(1) = matPose.m[1][0];
	matrixObj(2) = matPose.m[2][0];
	matrixObj(3) = 0.0;

	matrixObj(4) = matPose.m[0][1]; 
	matrixObj(5) = matPose.m[1][1]; 
	matrixObj(6) = matPose.m[2][1];
	matrixObj(7) = 0.0;

	matrixObj(8) = matPose.m[0][2]; 
	matrixObj(9) = matPose.m[1][2]; 
	matrixObj(10) = matPose.m[2][2];
	matrixObj(11) = 0.0;

	matrixObj(12) = matPose.m[0][3]; 
	matrixObj(13) = matPose.m[1][3]; 
	matrixObj(14) = matPose.m[2][3];
	matrixObj(15) = 1.0f;

	return matrixObj;
}

void GraphicsEngine::UpdateHMDMatrixPose()
{
	if (!DX11::m_pHMD)
		return;

	vr::VRCompositor()->WaitGetPoses(DX11::m_rTrackedDevicePose, vr::k_unMaxTrackedDeviceCount, NULL, 0);

	m_iValidPoseCount = 0;
	m_strPoseClasses = "";
	for (int nDevice = 0; nDevice < vr::k_unMaxTrackedDeviceCount; ++nDevice)
	{
		if (DX11::m_rTrackedDevicePose[nDevice].bPoseIsValid)
		{
			m_iValidPoseCount++;
			DX11::m_rmat4DevicePose[nDevice] = ConvertSteamVRMatrixToMatrix4(DX11::m_rTrackedDevicePose[nDevice].mDeviceToAbsoluteTracking);
			if (m_rDevClassChar[nDevice] == 0)
			{
				switch (DX11::m_pHMD->GetTrackedDeviceClass(nDevice))
				{
				case vr::TrackedDeviceClass_Controller:        m_rDevClassChar[nDevice] = 'C'; break;
				case vr::TrackedDeviceClass_HMD:               m_rDevClassChar[nDevice] = 'H'; break;
				case vr::TrackedDeviceClass_Invalid:           m_rDevClassChar[nDevice] = 'I'; break;
				case vr::TrackedDeviceClass_GenericTracker:    m_rDevClassChar[nDevice] = 'O'; break;
				case vr::TrackedDeviceClass_TrackingReference: m_rDevClassChar[nDevice] = 'T'; break;
				default:                                       m_rDevClassChar[nDevice] = '?'; break;
				}
			}
			m_strPoseClasses += m_rDevClassChar[nDevice];
		}
	}

	if (DX11::m_rTrackedDevicePose[vr::k_unTrackedDeviceIndex_Hmd].bPoseIsValid)
	{
		//m_mat4HMDPose = DX11::m_rmat4DevicePose[vr::k_unTrackedDeviceIndex_Hmd].invert();
	}
	else
	{
		printf("pose not valid");
	}
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



	/*if(myWantToResizeBuffers)
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
	}*/

	//Vector4f clearColor = Renderer::GetClearColor();
	//DX11::BeginFrame({ clearColor.x, clearColor.y, clearColor.z, clearColor.w });
	//
	//ResetStates();
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

void GraphicsEngine::RenderScene(vr::Hmd_Eye evr_eye)
{
	auto scene = SceneManager::GetScene();

	if (!scene)
	{
		return;
	}

	Matrix4x4f projection = Renderer::GetCamera()->GetCurrentViewProjectionMatrix(evr_eye);
	Matrix4x4f view = Renderer::GetCamera()->GetCurrentViewMatrix(evr_eye);



	std::vector<Light*>& someLightList = scene->GetLights();

	const std::shared_ptr<DirectionalLight>& directionalLight = scene->GetDirLight();
	const std::shared_ptr<EnvironmentLight>& environmentLight = scene->GetEnvLight();
	const std::vector<RenderBuffer>& modelList = Renderer::GetModels();
	std::vector<RenderBuffer2D>& spriteList = Renderer::GetSprites();

	float deltaTime = Time::GetDeltaTime();

	// TODO : CLEAN THIS MESS UP

	//if (GetRenderModeInt() != 9)
	//{
	//	{
	//		PROFILE_SCOPE("Render Shadows");
	//		myShadowRenderer->ClearResources();

	//		/*myShadowRenderer->Render(environmentLight.get(), modelList);
	//		myShadowRenderer->Render(directionalLight.get(), modelList);*/

	//		for (auto& light : someLightList)
	//		{
	//			myShadowRenderer->Render(light, modelList);
	//		}
	//		//myShadowRenderer->ClearResources();
	//	}
	//}
	//myShadowRenderer->ClearTarget();

	//RendererBase::SetDepthStencilState(DepthStencilState::ReadWrite);
	//RendererBase::SetBlendState(BlendState::None);

	//myGBuffer->ClearResource(0);
	//myGBuffer->SetAsTarget();

	//{
	//	PROFILE_SCOPE("Generate GBuffer");
	//	myDeferredRenderer->GenerateGBuffer(modelList, deltaTime, 0);
	//}

	//myGBuffer->ClearTarget();
	//myGBuffer->SetAsResource(0);


	//{
	//	PROFILE_SCOPE("Render With Deferred Renderer");
	//	RendererBase::SetBlendState(BlendState::Alpha);
	//	myDeferredRenderer->Render(view, projection, directionalLight, environmentLight, someLightList, deltaTime, 0);
	//	//myDeferredRenderer->ClearTarget();
	//}
	//myGBuffer->ClearTarget();

	//{
	//	PROFILE_SCOPE("Render SSAO");
	//	/*if (sssaoOn)
	//	{
	//		myPostProcessRenderer->Render(PostProcessRenderer::PP_SSAO);
	//	}
	//	else
	//	{
	//		myPostProcessRenderer->ClearTargets();
	//	}*/
	//}

	if (GetRenderModeInt() != 9)
	{
		{
			//PROFILE_SCOPE("Render With Forward Renderer (Models)");
			//RendererBase::SetDepthStencilState(DepthStencilState::ReadWrite);
			//RendererBase::SetBlendState(BlendState::None);

			myForwardRenderer->Render(view, projection, modelList, directionalLight, environmentLight, someLightList);
		}

		/*{
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
		RendererBase::SetBlendState(BlendState::Alpha);*/
	}

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

	DX11::m_RenderTextureLeft->SetRenderTarget(DX11::GetContext(), DX11::GetDepthStencilView());
	//Clear the render to texture background to blue so we can differentiate it from the rest of the normal scene.

		// Clear the render to texture.
	DX11::m_RenderTextureLeft->ClearRenderTarget(DX11::GetContext(), DX11::GetDepthStencilView(), 0.0f, 0.0f, 1.0f, 1.0f);

	// Render the scene now and it will draw to the render to texture instead of the back buffer.
	RenderScene(vr::Hmd_Eye::Eye_Left);
	


	// Set the render target to be the render to texture.
	DX11::m_RenderTextureRight->SetRenderTarget(DX11::GetContext(), DX11::GetDepthStencilView());
	//Clear the render to texture background to blue so we can differentiate it from the rest of the normal scene.

		// Clear the render to texture.
	DX11::m_RenderTextureRight->ClearRenderTarget(DX11::GetContext(), DX11::GetDepthStencilView(), 0.0f, 0.0f, 1.0f, 1.0f);

	// Render the scene now and it will draw to the render to texture instead of the back buffer.
	RenderScene(vr::Hmd_Eye::Eye_Right);
	

	// Reset the render target back to the original back buffer and not the render to texture anymore.
	DX11::GetContext()->OMSetRenderTargets(1, &DX11::myRenderTargetView, DX11::GetDepthStencilView());

	

	/*DX11::GetContext()->GSSetShader(nullptr, nullptr, 0);

	Renderer::Clear();*/

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


	//DX11::GetContext()->GSSetShader(nullptr, nullptr, 0);

	DX11::EndFrame();

	UpdateHMDMatrixPose();

	/*myGBuffer->Clear();
	myDropManager->ClearPaths();*/

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

