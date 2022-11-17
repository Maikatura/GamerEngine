// Exclude things we don't need from the Windows headers
#define WIN32_LEAN_AND_MEAN
#include "Windows.h"

#include "Modelviewer.h"
#include "GraphicsEngine.h"
#include "Scene/SceneSerializer.h"

#if EDITOR
#undef EDITOR
#endif

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                      _In_opt_ HINSTANCE hPrevInstance,
                      _In_ LPWSTR    lpCmdLine,
                      _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hInstance);
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    UNREFERENCED_PARAMETER(nCmdShow);

    MSG msg = { 0 };

    GraphicsEngine graphicsEngine;

    const SIZE windowSize = { 1920, 1080 };

    bool bShouldRun = graphicsEngine.Initialize(
		(GetSystemMetrics(SM_CXSCREEN) - windowSize.cx) / 2,
		(GetSystemMetrics(SM_CYSCREEN) - windowSize.cy) / 2,
		windowSize.cx,
		windowSize.cy,
		false,
        L"Cool",
        false
    );

    SceneSerializer loadScene(graphicsEngine.GetScene().get());
    loadScene.Deserialize("Assets\\Scenes\\shadowtesting.csf");
 
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

        graphicsEngine.BeginFrame();
        graphicsEngine.OnFrameUpdate(true);
        graphicsEngine.OnFrameRender();
        graphicsEngine.EndFrame();
    }



    return 0;
}
