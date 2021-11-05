#include <windows.h>

#define  MAX_NAME_STRING 256
#define HInstance() GetModuleHandle(NULL);

WCHAR WindowClass[MAX_NAME_STRING];
WCHAR WindowTitle[MAX_NAME_STRING];

INT WindowWidth;
INT WindowHeight;

int CALLBACK WinMain(HINSTANCE, HINSTANCE, LPSTR, INT)
{
    

    /* - Initialize Global Variables - */
    wcscpy_s(WindowClass, TEXT("GamerEngine"));
    wcscpy_s(WindowTitle, TEXT("GamerEngine"));

    WindowWidth = 1280;
    WindowHeight = 720;

    HINSTANCE instance = HInstance();

    /* - Create Window Class - */

    WNDCLASSEX wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;

    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);

    wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    wcex.lpszClassName = WindowClass;
    wcex.lpszMenuName = NULL;

    wcex.hInstance = instance;

    wcex.lpfnWndProc = DefWindowProc;

    RegisterClassEx(&wcex);

    /* - Create and Display our Window - */


    HWND hWnd = CreateWindow(WindowClass, WindowTitle,
        WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
        0, 0, WindowWidth, WindowHeight, NULL, NULL, instance, NULL);

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