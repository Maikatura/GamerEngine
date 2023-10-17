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


	if (!DX11::Init(myWindowHandle, enableDeviceDebug, isVRMode))
	{
		return false;
	}

	myForwardRenderer = std::make_shared<ForwardRenderer>();
	myDeferredRenderer = std::make_shared<DeferredRenderer>();
	myShadowRenderer = std::make_shared<ShadowRenderer>();
	myPostProcessRenderer = std::make_shared<PostProcessRenderer>();
	myGBuffer = std::make_shared<GBuffer>();

	if (!myForwardRenderer->Initialize())
	{
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

	Time::Update();
	Input::Init();
	AudioManager::Init();
	SceneManager::Get().Initialize();

	START_PROFILE("Start of Program");

	return true;
}

inline Matrix4x4f ConvertSteamVRMatrixToMatrix4(const vr::HmdMatrix34_t& matPose)
{
	Matrix4x4f matrixObj;
	for (int row = 0; row < 4; ++row) {
		for (int col = 0; col < 4; ++col) {
			matrixObj(row + 1, col + 1) = matPose.m[col][row]; // Transpose the matrix during conversion
		}
	}

	return matrixObj;
}

inline Matrix4x4f ConvertSteamVRMatrixToMatrix4(const vr::HmdMatrix44_t& matPose)
{
	Matrix4x4f matrixObj;

	for (int row = 0; row < 4; ++row) {
		for (int col = 0; col < 4; ++col) {
			matrixObj(row + 1, col + 1) = matPose.m[col][row]; // Transpose the matrix during conversion
		}
	}

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
		DX11::m_mat4HMDPose = Matrix4x4f::GetFastInverse(DX11::m_rmat4DevicePose[vr::k_unTrackedDeviceIndex_Hmd]);
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

	switch (uMsg)
	{
	case WM_CREATE:
	{
		const CREATESTRUCT* createdStruct = reinterpret_cast<CREATESTRUCT*>(lParam);
		graphicsEnginePtr = static_cast<GraphicsEngine*>(createdStruct->lpCreateParams);
	}

	case WM_SIZE:
	{
		if (DX11::Device != NULL)
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
			}
		}
	}

	case WM_EXITSIZEMOVE:
	{
		if (DX11::Device != NULL)
		{
			graphicsEnginePtr->SetUpdateBuffers(true);
		}
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



	if(myWantToResizeBuffers)
	{
		if(DX11::SwapChain)
		{
			if (SceneManager::Get().GetStatus() == SceneStatus::Complete)
			{
				myGBuffer->Release();
				DX11::Resize();
				auto scene = SceneManager::Get().GetScene();
				scene->Resize({ static_cast<unsigned int>(Get()->GetWindowSize().cx), static_cast<unsigned int>(Get()->GetWindowSize().cy) });
				myGBuffer->CreateGBuffer();
				//myPostProcessRenderer->ReInitialize();

			}


		}
		myWantToResizeBuffers = false;
	}

	//Vector4f clearColor = Renderer::GetClearColor();
	//DX11::BeginFrame({ clearColor.x, clearColor.y, clearColor.z, clearColor.w });
	//
	ResetStates();
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
#endif

	SceneManager::Get().GetScene()->OnUpdate((aShouldRunLoop && !myIsPaused), SceneManager::Get().GetStatus() == SceneStatus::Complete);
	

//#ifdef _Distribution
//	std::string fps = "FPS: " + std::to_string(Time::GetFPS());
//	std::cout << fps.c_str() << std::endl;
//#endif

	//}
}

void GraphicsEngine::RenderScene(VREye anEye)
{
	if (!SceneManager::Get().IsReady())
	{
		return;
	}

	auto scene = SceneManager::Get().GetScene();

	if (!scene)
	{
		return;
	}

	if (SceneManager::Get().GetStatus() != SceneStatus::Complete)
	{
		return;
	}
	

	Matrix4x4f projection = Renderer::GetCamera()->GetHMDMatrixProjectionEye(anEye);
	Matrix4x4f view = Renderer::GetCamera()->GetCurrentViewProjectionMatrix(anEye);


	std::vector<Light*>& someLightList = scene->GetLights();

	const std::shared_ptr<DirectionalLight>& directionalLight = scene->GetDirLight();
	const std::shared_ptr<EnvironmentLight>& environmentLight = scene->GetEnvLight();
	const std::vector<RenderBuffer>& modelList = Renderer::GetModels();
	std::vector<RenderBuffer2D>& spriteList = Renderer::GetSprites();

	float deltaTime = Time::GetDeltaTime();

	// TODO : CLEAN THIS MESS UP

	bool renderSSAO = true;
	if (Input::IsKeyDown('P'))
	{
		renderSSAO = false;
	}

	static bool sssaoOn = true;

	if (Input::IsKeyPressed('P'))
	{
		sssaoOn = !sssaoOn;
	}

	if (GetRenderModeInt() != 9)
	{
		{
			PROFILE_SCOPE("Render Shadows");
			myShadowRenderer->ClearResources();

			myShadowRenderer->Render(environmentLight.get(), modelList);
			myShadowRenderer->Render(directionalLight.get(), modelList);

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
		myDeferredRenderer->GenerateGBuffer(view, projection, modelList, deltaTime, 0, anEye);
	}

	myGBuffer->ClearTarget();
	myGBuffer->SetAsResource(0);

	{
		PROFILE_SCOPE("Render With Deferred Renderer");
		RendererBase::SetBlendState(BlendState::Alpha);
		myDeferredRenderer->Render(view, projection, directionalLight, environmentLight, someLightList, deltaTime, 0, anEye);
		myDeferredRenderer->ClearTarget();
	}
	myGBuffer->ClearTarget();

	

	/*{
		PROFILE_SCOPE("Render SSAO");
		if (sssaoOn)
		{
			myPostProcessRenderer->Render(PostProcessRenderer::PP_SSAO);
		}
		else
		{
			myPostProcessRenderer->ClearTargets();
		}
	}*/

	if (GetRenderModeInt() != 9)
	{
		{
			PROFILE_SCOPE("Render With Forward Renderer (Models)");
			RendererBase::SetDepthStencilState(DepthStencilState::ReadWrite);
			RendererBase::SetBlendState(BlendState::None);

			myForwardRenderer->Render(view, projection, modelList, directionalLight, environmentLight, someLightList, anEye);
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
		}*/



		DX11::ResetRenderTarget(myUseEditor);


		PROFILE_SCOPE("Final Render Call");
		myDeferredRenderer->RenderLate();
	}

}

void GraphicsEngine::OnFrameRender()
{
	if (myIsMinimized) return;


	if (!SceneManager::Get().IsReady())
	{
		return;
	}

	bool renderSSAO = true;
	if (Input::IsKeyDown('P'))
	{
		renderSSAO = false;
	}

	static bool sssaoOn = true;

	if (Input::IsKeyPressed('P'))
	{
		sssaoOn = !sssaoOn;
	}


	auto scene = SceneManager::Get().GetScene();

	if (!scene)
	{
		return;
	}

	if (!scene->IsReady())
	{
		return;
	}

	OnFrameUpdate(GetEngineUpdateRuntime());
	scene->OnRender();

#ifndef VR_DISABLED

	if (!myIsPaused)
	{




		DX11::GetContext()->RSSetState(DX11::myFrontCulling);

		DX11::m_RenderTextureLeft->SetRenderTarget(DX11::GetContext(), DX11::GetDepthStencilView());
		//Clear the render to texture background to blue so we can differentiate it from the rest of the normal scene.

			// Clear the render to texture.
		DX11::m_RenderTextureLeft->ClearRenderTarget(DX11::GetContext(), DX11::GetDepthStencilView(), 0.0f, 0.0f, 1.0f, 1.0f);

		// Render the scene now and it will draw to the render to texture instead of the back buffer.
		RenderScene(VREye::Left);



		// Set the render target to be the render to texture.
		DX11::m_RenderTextureRight->SetRenderTarget(DX11::GetContext(), DX11::GetDepthStencilView());
		//Clear the render to texture background to blue so we can differentiate it from the rest of the normal scene.

			// Clear the render to texture.
		DX11::m_RenderTextureRight->ClearRenderTarget(DX11::GetContext(), DX11::GetDepthStencilView(), 0.0f, 0.0f, 1.0f, 1.0f);

		// Render the scene now and it will draw to the render to texture instead of the back buffer.
		RenderScene(VREye::Right);
	}

#endif
	//// Reset the render target back to the original back buffer and not the render to texture anymore.





	if (myUseEditor)
	{

		//DX11::TurnZBufferOff();
		DX11::myScreenView->SetRenderTarget(DX11::GetContext(), DX11::GetDepthStencilView());
		//Clear the render to texture background to blue so we can differentiate it from the rest of the normal scene.

		// Clear the render to texture.
		DX11::myScreenView->ClearRenderTarget(DX11::GetContext(), DX11::GetDepthStencilView(), 0.0f, 0.0f, 1.0f, 1.0f);
	}
	else
	{
		//DX11::TurnZBufferOff();
		//DX11::GetContext()->RSSetState(DX11::myBackCulling);
		Vector4f clearColor = Renderer::GetClearColor();
		clearColor.z = 1.0f;
		clearColor.w = 1.0f;

		DX11::GetContext()->OMSetRenderTargets(1, &DX11::myRenderTargetView, DX11::GetDepthStencilView());
		float clearDepth = 1.0f;  // The value to which you want to clear the depth buffer
		UINT8 clearStencil = 0;  // The value to which you want to clear the stencil buffer (if you have one)

		DX11::GetContext()->ClearDepthStencilView(DX11::GetDepthStencilView(), D3D11_CLEAR_DEPTH, clearDepth, clearStencil);
		DX11::GetContext()->ClearRenderTargetView(DX11::myRenderTargetView, &clearColor.x);

	}



	RenderScene(VREye::None);




	/*DX11::GetContext()->GSSetShader(nullptr, nullptr, 0);

	Renderer::Clear();*/



	scene->Clean();
}

void GraphicsEngine::StartUpdateThread()
{

	/*myUpdateThread = std::make_shared<std::thread>([&]()
		{
		});*/

	myIsRunning = true;

}

void GraphicsEngine::StopUpdateThread()
{
	if (myUpdateThread)
	{
		myIsRunning = false;

		while (!myUpdateThread->joinable())
		{

		}

		if (myUpdateThread && myUpdateThread->joinable()) {
			myUpdateThread->join();
		}
	}
}

void GraphicsEngine::EndFrame()
{
	if (myIsMinimized) return;


	DX11::GetContext()->GSSetShader(nullptr, nullptr, 0);

	DX11::EndFrame();

	UpdateHMDMatrixPose();

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

