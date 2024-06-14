#pragma once
#include "Utilites/KeyCodes.h"
#include "MouseCursor.h"
#include "Math/MathTypes.hpp"
#include <bitset>

namespace CommonUtilities
{
	class MouseInput
	{
	public:
		explicit MouseInput(HWND aWindowHandle);
		~MouseInput() = default;
		bool IsMousePressed(Mouse::Button aMouseButton) const;
		bool WasMousePressedThisFrame(Mouse::Button aMouseButton) const;
		bool IsMouseReleased(Mouse::Button aMouseButton) const;
		bool WasMouseReleasedThisFrame(Mouse::Button aMouseButton) const;
		bool ProcessEvents(UINT aMessage, WPARAM aWParam, LPARAM aLParam);
		void Update();

		const Vector2f& GetScrollValue() const { return myCurrentMouseScrollValue; }

		void ResetInput();
	private:
		HWND myOwnerWindowHandle { nullptr };
		std::bitset<Mouse::ButtonsCount> myPreviousMouseState {};
		std::bitset<Mouse::ButtonsCount> myCurrentMouseState {};
		std::bitset<Mouse::ButtonsCount> myNextMouseState {};
		Vector2f myCurrentMouseScrollValue { 0.0f, 0.0f };
		Vector2f myNextMouseScrollValue { 0.0f, 0.0f };
	};
}
