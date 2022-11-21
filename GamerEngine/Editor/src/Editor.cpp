// Exclude things we don't need from the Windows headers
#include "Editor.pch.h"

#include <d3d11.h>
#include <Editor.h>
#include <GraphicsEngine.h>
#include <Framework/DX11.h>
#include <Render/LineRenderer.h> 
#include "Time.hpp"
#include "Layers/EditorLayers.h"
#include "Layers/UI/EditorSettingsPanel.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                      _In_opt_ HINSTANCE hPrevInstance,
                      _In_ LPWSTR    lpCmdLine,
                      _In_ int       nCmdShow)
{
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

	UNREFERENCED_PARAMETER(hInstance);
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	UNREFERENCED_PARAMETER(nCmdShow);

	MSG msg = { 0 };

	GraphicsEngine graphicsEngine;
	EditorLayers myLayers;

	const SIZE windowSize = { 1920, 1080 };

	std::wstring editorName = L"GamerEngine Editor";
#if _DEBUG
	editorName += L" DEBUG";
#endif

	bool bShouldRun = graphicsEngine.Initialize(
		(GetSystemMetrics(SM_CXSCREEN) - windowSize.cx) / 2,
		(GetSystemMetrics(SM_CYSCREEN) - windowSize.cy) / 2,
		windowSize.cx,
		windowSize.cy,
		true,
		editorName,
	true);

	myLayers.Init();
	EditorSettingsPanel::LoadConfig();
	LineRenderer::Init();
	
	while(bShouldRun)
	{
		while(PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if(msg.message == WM_QUIT)
			{
				myLayers.Destory();
				bShouldRun = false;
			}
		}

		if(!bShouldRun)
		{
			break;
		}


		myLayers.BeginFrame();
		graphicsEngine.BeginFrame();
		graphicsEngine.OnFrameUpdate(myLayers.ShouldRunEngine());
		graphicsEngine.OnFrameRender();

		LineRenderer::Render();
		LineRenderer::Clear();

		myLayers.OnUpdate();
		myLayers.OnImGuiRender();

		myLayers.EndFrame();
		graphicsEngine.EndFrame();
	}

	
	//std::cout << Allocation::TotalAlloc() << std::endl;
	//system("pause");

#if _DEBUG
#pragma warning( push )
#pragma warning( disable : 4996 )
	fclose(stdin);
	fclose(stdout);
	fclose(stderr);
#pragma warning( pop )
#endif


	return 0;
}
