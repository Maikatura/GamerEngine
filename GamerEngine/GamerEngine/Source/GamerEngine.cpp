#include <Windows.h>
#include "Core/Vulkan/GamerEngineCore.h"
#include "Core/Vulkan/GamerInit.hpp"
#include "Core/Vulkan/GamerSwapchain.h"

#define  MAX_NAME_STRING 256
#define HInstance() GetModuleHandle(NULL)

WCHAR WindowClass[MAX_NAME_STRING];
WCHAR WindowTitle[MAX_NAME_STRING];

INT WindowWidth;
INT WindowHeight;

static bool myEngineIsRunning = true;
GamerEngine::GamerEngine myEngine;

LRESULT CALLBACK WindowProcess(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        case WM_DESTROY:
            myEngineIsRunning = false;
            PostQuitMessage(0);
            break;

        case WM_SIZE:
            myEngine.UpdateSwapchain();
            std::cout << "Window size changed" << std::endl;
            break;
    }

    return DefWindowProc(hWnd, message, wParam, lParam);
}

//int CALLBACK WinMain(HINSTANCE, HINSTANCE, LPSTR, INT)
//{
//   
//}


int main()
{
    /* - Initialize Global Variables - */
    wcscpy_s(WindowClass, TEXT("GamerEngine"));
    wcscpy_s(WindowTitle, TEXT("GamerEngine"));

    WindowWidth = 1280;
    WindowHeight = 720;

    /* - Create Window Class - */

    WNDCLASSEX wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;

    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);

    wcex.hIcon = LoadIcon(0, IDI_APPLICATION);
    wcex.hIconSm = LoadIcon(0, IDI_APPLICATION);

    wcex.lpszClassName = WindowClass;
    wcex.lpszMenuName = nullptr;

    wcex.hInstance = HInstance();

    wcex.lpfnWndProc = WindowProcess;

    RegisterClassEx(&wcex);

    /* - Create and Display our Window - */


    HWND hWnd = CreateWindow(WindowClass, WindowTitle,
        WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
        0, 0, WindowWidth, WindowHeight, nullptr, nullptr, HInstance(), nullptr);

    if (!hWnd)
    {
        MessageBox(0, L"Failed to Create Window.", 0, 0);
        return 0;
    }

    ShowWindow(hWnd, SW_SHOW);

    bool engineStartFailed = myEngine.Init(hWnd);
#if _DEBUG
    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize = ImVec2((float)WindowWidth, (float)WindowHeight);
#endif
    if (!engineStartFailed)
    {
        std::cout << "Failed to Initialize Vulkan" << std::endl;
        return -1;
    }

    while (myEngineIsRunning)
    {
    MSG msg = { 0 };
    while (msg.message != WM_QUIT)
    {
        if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        if (myEngineIsRunning)
        {
            if (!myEngine.Update())
            {
                std::cout << "Failed to Render" << std::endl;
                return -1;
            }

#if _DEBUG
           /* ImGui_ImplVulkan_NewFrame();
            ImGui_ImplWin32_NewFrame();
            ImGui::NewFrame();
            ImGui::ShowDemoWindow();
            ImGui::Render();*/
#endif
        }
    }
   
	    
    }

    myEngine.DestroyInstance();

    return 0;
}