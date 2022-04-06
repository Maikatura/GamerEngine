#include <filesystem>
#include <fstream>
#include <windows.h>

#include "Vulkan/platform.h"

#include "Vulkan/vk_renderer.h"

global_variable bool running = true;
global_variable HWND window;

LRESULT CALLBACK platform_window_callback(HWND window, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_CLOSE:
        running = false;
        break;
    }

    return DefWindowProcA(window, msg, wParam, lParam);
}

bool platform_create_window()
{
    HINSTANCE instance = GetModuleHandleA(0);

    WNDCLASSA wc = {};
    wc.lpfnWndProc = platform_window_callback;
    wc.hInstance = instance;
    wc.lpszClassName = "vulkan_engine_class";
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);

    if (!RegisterClassA(&wc))
    {
        MessageBoxA(0, "Failed registering window class", "Error", MB_ICONEXCLAMATION | MB_OK);
        return false;
    }

    window = CreateWindowExA(
        WS_EX_APPWINDOW,
        "vulkan_engine_class",
        "Pong",
        WS_THICKFRAME | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_OVERLAPPED,
        100, 100, 1600, 720, 0, 0, instance, 0);

    if (window == 0)
    {
        MessageBoxA(0, "Failed creating window", "Error", MB_ICONEXCLAMATION | MB_OK);
        return false;
    }

    ShowWindow(window, SW_SHOW);

    return true;
}

void platform_update_window(HWND window)
{
    MSG msg;

    while (PeekMessageA(&msg, window, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }
}

int main()
{
    VkContext vkcontext = {};

    if (!platform_create_window())
    {
        return -1;
    }

    if (!vk_init(&vkcontext, window))
    {
        return -1;
    }

    while (running)
    {
        platform_update_window(window);
        if (!vk_render(&vkcontext))
        {
            return -1;
        }
    }

    return 0;
}

// ########################################################################
//                   Implementation of exposed platform functions
// ########################################################################

void platform_get_window_size(uint32_t *width, uint32_t *heigth)
{
    RECT rect;
    GetClientRect(window, &rect);

    *width = rect.right - rect.left;
    *heigth = rect.bottom - rect.top;
}

char* platform_read_file(const char* path, int* length)
{
    std::fstream file;
    file.open(path, std::ios_base::in | std::ios::binary);

    if (!file.is_open()) {
        file.clear();
        file.open(path, std::ios_base::out | std::ios_base::binary);
        file.close();
        file.open(path, std::ios_base::in | std::ios_base::binary);

        std::cout << "a new file was created!" << std::endl;
    } else if (file.good()) {
        std::cout << "file already exists." << std::endl;
        std::cout << "file was opened on the first try!" << std::endl;
    }
    auto file_size = std::filesystem::file_size(path);
    char *data = reinterpret_cast<char *>(new int[file_size]);
    file.read(data, file_size);
    std::string output(data);
    std::cout << output << std::endl;
    return data;
}