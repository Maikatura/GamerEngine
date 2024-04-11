#pragma once
#include <bitset>
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <Windows.h>

#include "Math/MathTypes.hpp"
#include "Math/Vector3.hpp"

namespace CommonUtilities
{

    class InputManager
    {
    public:
        InputManager(HWND aHWND);
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
        void SetMousePos(Vector2i aPos);
        Vector2i GetMousePos();

    private:
        HWND myHWND;

        std::bitset<7> myCurrentMousePress;
        std::bitset<7> myPreviousMousePress;
        std::bitset<7> mySavedMousePress;

        float myCurrentScrollDelta;
        float myLastScrollDelta;
        float mySavedScrollDelta;

        POINT myMousePoint;

        Vector2f mySavedMousePosition;
        Vector2f myPreviousMousePosition;
        Vector2f myCurrentMousePosition;
        Vector2f myMouseDelta;

        Vector2f myCurrentRelativeMousePosition;

        std::bitset<256> mySavedWindowsKeyboardState;
        std::bitset<256> myCurrentKeyboardState;
        std::bitset<256> myPreviousKeyboardState;

        bool myWindowIsActive = false;
    };
}