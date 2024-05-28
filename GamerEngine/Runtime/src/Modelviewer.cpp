// Exclude things we don't need from the Windows headers





#ifdef GE_PLATFORM_WINDOWS
    #ifndef NOMINMAX
        // See github.com/skypjack/entt/wiki/Frequently-Asked-Questions#warning-c4003-the-min-the-max-and-the-macro
        #define NOMINMAX
    #endif
#endif



#ifdef GE_NETWORK

#ifdef GE_PLATFORM_WINDOWS
#define WIN32_LEAN_AND_MEAN
#endif
#endif


#ifdef GE_PLATFORM_WINDOWS
#include <Windows.h>
#endif



// Don't worry this runtime doesn't have a fee and never will c:

#include "Modelviewer.h"
#include "GraphicsEngine.h"
#include "Physics.h"
#include "Core/Rendering/LineRenderer.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "Scene/SceneSerializer.h"
#include "Utilites/VisualProfiler.h"


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                      _In_opt_ HINSTANCE hPrevInstance,
                      _In_ LPWSTR    lpCmdLine,
                      _In_ int       nCmdShow)
{

    START_PROFILE_WITH_PATH("Game Profile", "startup_game_profile.json");

    UNREFERENCED_PARAMETER(hInstance);
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    UNREFERENCED_PARAMETER(nCmdShow);


#ifndef _Distribution
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

    MSG msg = { 0 };

    GraphicsEngine graphicsEngine;

    const SIZE windowSize = { 1280, 720 };

    bool bShouldRun = graphicsEngine.Initialize(
		(GetSystemMetrics(SM_CXSCREEN) - windowSize.cx) / 2,
		(GetSystemMetrics(SM_CYSCREEN) - windowSize.cy) / 2,
		windowSize.cx,
		windowSize.cy,
		false,
        L"Cool",
        false,
        true
    );

    Physics::Get().Init();
    SceneManager::Get().Init();
    SceneManager::Get().LoadScene("Editor\\Scenes\\default.csf");

    STOP_PROFILE();

    while(bShouldRun)
    {
		while(PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
            TranslateMessage(&msg);
            DispatchMessage(&msg);

			if(msg.message == WM_QUIT)
			{
                bShouldRun = false;
			}
		}

        if(!bShouldRun)
        {
            break;
        }


        graphicsEngine.BeginFrame();
        graphicsEngine.OnFrameUpdate();
        graphicsEngine.OnFrameRender();
        graphicsEngine.EndFrame();
    }

#ifndef _Distribution
#pragma warning( push )
#pragma warning( disable : 4996 )
    fclose(stdin);
    fclose(stdout);
    fclose(stderr);
#pragma warning( pop )
#endif
    return 0;
}
