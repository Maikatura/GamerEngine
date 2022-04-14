#pragma once
#include <bitset>
#include <Windows.h>
#include "Vector3.hpp"

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
        Vector3<float> MouseDelta() const;
        bool IsScrolling() const;
        float ScrollDelta() const;
        POINT GetMousePos() const;

        void Update();
        bool UpdateEvents(UINT message, WPARAM wParam, LPARAM lParam);


    private:
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

        std::bitset<256> mySavedWindowsKeyboardState;
        std::bitset<256> myCurrentKeyboardState;
        std::bitset<256> myPreviousKeyboardState;
    };
}