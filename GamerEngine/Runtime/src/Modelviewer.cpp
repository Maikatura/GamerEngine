// Exclude things we don't need from the Windows headers


#define WIN32_LEAN_AND_MEAN
#define _WINSOCKAPI_ 
#include "Windows.h"



#include "Modelviewer.h"
#include "GraphicsEngine.h"
#include "Render/LineRenderer.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "Scene/SceneSerializer.h"


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                      _In_opt_ HINSTANCE hPrevInstance,
                      _In_ LPWSTR    lpCmdLine,
                      _In_ int       nCmdShow)
{
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

    SceneManager::Get().Initialize();
    SceneManager::Get().LoadScene("Assets\\Warcraft_Test\\Scene\\Skeleton.csf");
 
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
        graphicsEngine.OnFrameUpdate(true);
        graphicsEngine.OnFrameRender();
        graphicsEngine.EndFrame();



		#ifndef _Distribution
        //LineRenderer::Get().Clear();
		#endif
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
