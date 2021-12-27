#include "pch/pch.h"

WCHAR WindowClass[MAX_NAME_STRING];
WCHAR WindowTitle[MAX_NAME_STRING];

INT WindowWidth;
INT WindowHeight;

LRESULT CALLBACK WindowProcess(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch(message)
    {
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
    }

    return DefWindowProc(hWnd, message, wParam, lParam);
}

int CALLBACK WinMain(HINSTANCE, HINSTANCE, LPSTR, INT)
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

    /* - List for message events - */

    MSG msg = {0};
    while (msg.message != WM_QUIT)
    {
        if (PeekMessage(&msg, 0,0,0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return 0;

}