// Exclude things we don't need from the Windows headers
#include "Editor.pch.h"

#include "TurHeader.h"
#define _WINSOCKAPI_ 
#include "Windows.h"


#include <Framework/DX11.h>
#include <GraphicsEngine.h>
#include <Framework/DX11.h>
#include <Render/LineRenderer.h> 

#include "Physics.h"
#include "Time.hpp"
#include "Components/CameraController.h"
#include "Components/NativeScriptComponent.h"
#include "Layers/EditorLayers.h"
#include "Layers/UI/EditorSettingsPanel.h"
#include "model/Entity.h"
#include "Scene/SceneManager.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


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

	std::function<bool(HWND, UINT, WPARAM, LPARAM)> function = [&](HWND aHWND, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		if(ImGui_ImplWin32_WndProcHandler(aHWND, uMsg, wParam, lParam))
			return true;

		return false;
	};

	graphicsEngine.SetWinProc(function);

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
	Physics::Get().Init();


	auto cameraFunc = std::function<void(Entity)>([](Entity aEntity)
	{
		aEntity.AddComponent<CameraControllerData>();
		aEntity.AddComponent<NativeScriptComponent>().Bind<CameraController>();
	});
	
	Scene::SetCameraHandle(cameraFunc);

	SceneManager::Get().LoadScene("Editor\\Scenes\\default.csf");
	
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

		//Physics::Get().Update();

		//graphicsEngine.SetEngineUpdateRuntime(myLayers.ShouldRunEngine());
		graphicsEngine.OnFrameUpdate(myLayers.ShouldRunEngine());
		graphicsEngine.OnFrameRender();

		/*LineRenderer::Render();
		LineRenderer::Clear();*/

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
