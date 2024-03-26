#pragma once
#include <bitset>
#include <Windows.h>

#include "Math/MathTypes.hpp"
#include "Math/Vector3.hpp"

namespace CommonUtilities
{

    class InputManager
    {
    public:
        InputManager();
        ~InputManager();

        bool IsKeyDown(const int aKeyCode) const;
        bool IsKeyUp(const int aKeyCode) const;
        bool IsKeyPressed(const int aKeyCode) const;
        bool IsKeyReleased(const int aKeyCode) const;

        bool IsMouseDown(const int aMouseKeyCode) const;
        bool IsMousePressed(const int aMouseKeyCode) const;
        bool IsMouseReleased(const int aMouseKeyCode) const;

        bool IsMouseMoving() const;
        Vector2f MouseDelta();
        bool IsScrolling() const;
        float ScrollDelta();
        POINT GetMousePos() const;
        POINT GetMousePosRelativeToWindow() const; 

        void Update();
        bool UpdateEvents(UINT message, WPARAM wParam, LPARAM lParam);
        void SetMousePos(int aX, int aY);
        Vector2i GetMousePos();

    private:
        HWND myHWND;

        std::bitset<3> myCurrentMousePress;
        std::bitset<3> myPreviousMousePress;
        std::bitset<3> mySavedMousePress;

        float myCurrentScrollDelta;
        float myLastScrollDelta;
        float mySavedScrollDelta;

        POINT myMousePoint;

        Vector3<float> mySavedMousePosition;
        Vector3<float> myPrevioustMousePosition;
        Vector3<float> myCurrentMousePosition;
        Vector2f myMouseDelta;

        Vector3<float> myCurrentRelativeMousePosition;

        std::bitset<256> mySavedWindowsKeyboardState;
        std::bitset<256> myCurrentKeyboardState;
        std::bitset<256> myPreviousKeyboardState;
    };
}