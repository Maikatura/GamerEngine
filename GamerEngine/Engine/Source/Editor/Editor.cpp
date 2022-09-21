// Exclude things we don't need from the Windows headers
#define WIN32_LEAN_AND_MEAN
#include "Windows.h"

#include <Editor.h>
#include <GraphicsEngine.h>
#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_dx11.h>
#include <ImGui/imgui_impl_win32.h>
#include <ImGui/ImGuizmo.h>
#include <Framework/DX11.h>
#include <Layers/LayerHeader.h>
#include <d3d11.h>
#include <Layers/EditorLayers.h>

#include "Time.hpp"



void SetUpImGuiStyle();

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                      _In_opt_ HINSTANCE hPrevInstance,
                      _In_ LPWSTR    lpCmdLine,
                      _In_ int       nCmdShow){
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
        editorName
    );
    
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    ImGui::StyleColorsDark();

    ImGui_ImplWin32_Init(graphicsEngine.GetWindowHandle());
    ImGui_ImplDX11_Init(DX11::Device.Get(), DX11::Context.Get());

    myLayers.AddLayer(std::make_shared<MainMenuBar>(&myLayers));
    myLayers.AddLayer(std::make_shared<FileExplorer>());
    myLayers.AddLayer(std::make_shared<Hierarchy>());
    myLayers.AddLayer(std::make_shared<EditorView>());
    myLayers.AddLayer(std::make_shared<Inspector>());

    EditorSettingsPanel::LoadConfig();

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

        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
        ImGuizmo::SetOrthographic(false);

        ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);
    	ImGuizmo::BeginFrame();

        graphicsEngine.BeginFrame();
        graphicsEngine.RenderFrame();

		DX11::Context->OMSetRenderTargets(1, DX11::BackBuffer.GetAddressOf(), NULL);

        DX11::Context->PSSetShaderResources(0, 1, DX11::RenderSRV.GetAddressOf());

        myLayers.OnImGuiRender();

        ImGui::Render();
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
        if(io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
        }

        graphicsEngine.EndFrame();
    }

    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    return 0;
}
