#include "GraphicsEngine.pch.h"
#include "GraphicsEngine.h"
#include <Core/Framework/DX11.h>
#include <Scene/Scene.h>
#include "Audio/AudioManager.h"
#include "Input/Input.h"
#include "Core/Rendering/Renderer.h"
#include "Managers/DropManager.h"
#include <Managers/CommandManager.h>
#include <Core/Rendering/ForwardRenderer.h>
//#include "Profiler/Profiler.h"
#include "Core/Rendering/DeferredRenderer.h"
#include "Core/Rendering/LineRenderer.h"
#include "Core/Rendering/ShadowRenderer.h"
#include "Core/Rendering/PostProcessRenderer.h"
#include "Scene/SceneManager.h"
#include "Font/Font.h"
#include "Project/Project.h"
#include "Scripting/ScriptEngine.h"


GraphicsEngine* GraphicsEngine::Get()
{
    return myInstance;
}

GraphicsEngine::~GraphicsEngine()
{
    GamerEngine::ScriptEngine::Shutdown();

    //StopUpdateThread();
    //
    //myUpdateShouldRun = false;
    //myUpdateThread.join();

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

    myFont = MakeRef<GamerEngine::Font>("resources\\fonts\\OpenSans.ttf");


    PostProcessRenderer::Get().Initialize();

    AddRenderModule<ShadowRenderer>();
    AddRenderModule<DeferredRenderer>();
    AddRenderModule<LineRenderer>();

    // TODO : Get rid of this RendererBase
    if (!RendererBase::Init())
    {
        return false;
    }

    Time::Update();
    Input::Init(myWindowHandle);

    GamerEngine::Project::New();
    AudioManager::Init();
    SceneManager::Get().Initialize();
    GamerEngine::ScriptEngine::Init();


    if (!myUseEditor)
    {
        //gCPUProfiler.Initialize(1, 512);
    }


    //myUpdateThread = std::thread(&GraphicsEngine::OnFrameUpdate, this);

    return true;
}

template <class T>
void GraphicsEngine::AddRenderModule()
{
    Ref<RenderModule> aModule = MakeRef<T>();
    if (!aModule->OnAdd())
    {
        GE_ASSERT(false, "ERROR Adding Render Module");
    }
    myRenderModules.push_back(aModule);
}

Ref<RenderModule> GraphicsEngine::GetRenderModule(int aModuleIndex)
{
    return myRenderModules[aModuleIndex];
}

void GraphicsEngine::RemoveRenderModule(int aModuleIndex)
{
    if (static_cast<int>(myRenderModules.size()) <= aModuleIndex)
    {
        return;
    }

    myRenderModules.erase(myRenderModules.begin() + aModuleIndex);
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
    default: ;
    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void GraphicsEngine::BeginFrame()
{
    if (myIsMinimized) return;

    if (!myUseEditor)
    {
        //PROFILE_FRAME();
    }

    ExecuteMainThreadQueue();

    if (myWantToResizeBuffers)
    {
        if (DX11::Get().GetSwapChain())
        {
            if (SceneManager::Get().GetStatus() == GamerEngine::SceneStatus::Complete)
            {
                for (auto module : myRenderModules)
                {
                    module->OnRelease();
                }

                //myGBuffer->Release();
                DX11::Get().Resize();
                const auto scene = SceneManager::Get().GetScene();
                scene->Resize({
                    static_cast<unsigned int>(Get()->GetWindowSize().cx),
                    static_cast<unsigned int>(Get()->GetWindowSize().cy)
                });
                //myGBuffer->CreateGBuffer();

                for (auto module : myRenderModules)
                {
                    module->OnAdd();
                }

                PostProcessRenderer::Get().ReInitialize();
            }
        }
        myWantToResizeBuffers = false;
    }


    //Vector4f clearColor = Renderer::GetClearColor();
    //DX11::Get().BeginFrame({ clearColor.x, clearColor.y, clearColor.z, clearColor.w });
    //
    ResetStates();

    Vector4f clearColor = Renderer::GetClearColor();

    const auto renderTarget = DX11::Get().GetRenderTargetView();
    DX11::Get().GetContext()->OMSetRenderTargets(1, &renderTarget, DX11::Get().GetDepthStencilView()->myDSV.Get());
    constexpr float clearDepth = 1.0f;
    constexpr UINT8 clearStencil = 0;

    DX11::Get().GetContext()->ClearDepthStencilView(DX11::Get().GetDepthStencilView()->myDSV.Get(), D3D11_CLEAR_DEPTH,
                                                    clearDepth, clearStencil);
    DX11::Get().GetContext()->ClearRenderTargetView(DX11::Get().GetRenderTargetView(), &clearColor.x);
}

void GraphicsEngine::OnFrameUpdate()
{
    //while (myUpdateShouldRun)
    //{
    Input::Update();
    Time::Update();


    if (myIsMinimized) return;

    if (!SceneManager::Get().IsReady())
    {
        return;
    }


    if (myIsPaused)
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


#if defined(_DEBUG) || defined(_RELEASE)
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

    {
        //PROFILE_CPU_SCOPE("Update Loop");

        if (SceneManager::Get().GetScene())
        {
            SceneManager::Get().Update();

            for (auto module : myRenderModules)
            {
                module->OnUpdate();
            }
        }
    }

    //#ifdef _Distribution
    //	std::string fps = "FPS: " + std::to_string(Time::GetFPS());
    //	std::cout << fps.c_str() << std::endl;
    //#endif

    //}

    //		while (myUpdateShouldRun)
    //		{
    //			if (myRenderIsDone)
    //			{
    				Renderer::SwapBuffers();
    //				myRenderIsDone = false;
    //				break;
    //			}
    //		}
    //
    //}
}

void GraphicsEngine::RenderScene(const VREye anEye) const
{
    if (!SceneManager::Get().IsReady())
    {
        return;
    }

    const Ref<GamerEngine::Scene> scene = SceneManager::Get().GetScene();

    if (!scene)
    {
        return;
    }

    if (SceneManager::Get().GetStatus() != GamerEngine::SceneStatus::Complete)
    {
        return;
    }

    if (!Renderer::GetCamera())
    {
        return;
    }


    for (auto module : myRenderModules)
    {
        module->OnRenderSetup();
        module->OnRender();
    }

    //Renderer::RenderString("Test", myFont, Matrix4x4f(1.0f), Vector4f(1.0f));
}

void GraphicsEngine::OnFrameRender()
{
    if (myWantToResizeBuffers) return;
    if (myIsMinimized) return;


    // if (myRenderIsDone)
    // {
    //     return;
    // }

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
        //PROFILE_CPU_SCOPE("Scene Render Setup");
        scene->OnRender();
    }


#if ENABLE_VR

	if (!myIsPaused)
	{

		DX11::Get().GetContext()->RSSetState(DX11::Get().GetFrontCulling());
		{
			//PROFILE_CPU_SCOPE("Render Left Eye (VR)");
			DX11::Get().GetRightEyeView()->SetRenderTarget(DX11::Get().GetContext(), DX11::Get().GetDepthStencilView());
			//Clear the render to texture background to blue so we can differentiate it from the rest of the normal scene.

				// Clear the render to texture.
			DX11::Get().GetRightEyeView()->ClearRenderTarget(DX11::Get().GetContext(), DX11::Get().GetDepthStencilView(), 0.0f, 0.0f, 1.0f, 1.0f);

			// Render the scene now and it will draw to the render to texture instead of the back buffer.
			RenderScene(VREye::Left);
		}

		{
			//PROFILE_CPU_SCOPE("Render Right Eye (VR)");

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
        //PROFILE_CPU_SCOPE("View Render Setup");
        if (myUseEditor)
        {
            //DX11::Get().TurnZBufferOff();
            DX11::Get().GetScreenView()->SetRenderTarget(DX11::Get().GetContext(),
                                                         DX11::Get().GetDepthStencilView()->myDSV.Get());
            DX11::Get().GetScreenView()->ClearRenderTarget(DX11::Get().GetContext(),
                                                           DX11::Get().GetDepthStencilView()->myDSV.Get(), 0.5f, 0.5f,
                                                           0.5f, 1.0f);
        }
        else
        {
            //DX11::Get().TurnZBufferOff();
            //DX11::Get().GetContext()->RSSetState(DX11::Get().myBackCulling);
        }
    }

    {
        //PROFILE_CPU_SCOPE("Render Screen");
        RenderScene(VREye::None);
    }


    /*DX11::Get().GetContext()->GSSetShader(nullptr, nullptr, 0);

    Renderer::Clear();*/


    {
        //PROFILE_CPU_SCOPE("Clean Scene");
        scene->Clean();
    }

    //myRenderIsDone = true;
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

void GraphicsEngine::SubmitToMainThread(const std::function<void()>& function)
{
    std::scoped_lock<std::mutex> lock(myMainThreadQueueMutex);
    myMainThreadQueue.emplace_back(function);
}

void GraphicsEngine::ExecuteMainThreadQueue()
{
    std::scoped_lock<std::mutex> lock(myMainThreadQueueMutex);

    for (auto& func : myMainThreadQueue)
        func();

    myMainThreadQueue.clear();
}

void GraphicsEngine::EndFrame() const
{
    if (myIsMinimized) return;


    {
        //	PROFILE_CPU_SCOPE("Final Render Call");
        DX11::Get().ResetRenderTarget(myUseEditor);
        for (auto module : myRenderModules)
        {
            module->OnEnd();
        }
    }

    {
        //PROFILE_CPU_SCOPE("End Of Frame");
        DX11::Get().EndFrame();
    }


    //myGBuffer->Clear();


    //myDropManager->ClearPaths();

    if (SceneManager::Get().GetStatus() == GamerEngine::SceneStatus::NeedSwap)
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
    //PROFILE_CPU_SCOPE("Reset States");
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

Vector2ui GraphicsEngine::GetEditorWindowSize() const
{
    return myEditorWindowSize;
}

void GraphicsEngine::SetEditorWindowSize(Vector2ui aEditorWindowSize)
{
    myEditorWindowSize = aEditorWindowSize;

    //myPostProcessRenderer->ReInitialize();
}
