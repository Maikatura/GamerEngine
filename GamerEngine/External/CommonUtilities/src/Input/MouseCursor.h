#pragma once
#include "Math/MathTypes.hpp"
#include "Windows.h"
#include "Utilites/WindowsUtils.h"

namespace CommonUtilities
{
	enum class CursorLockMode
	{
		None,
		Locked,
		LockedToWindow
	};

	class MouseCursor
	{
		public:
			MouseCursor(HWND aWindowHandle);
			~MouseCursor() = default;

			void Update();

			bool ProcessEvents(UINT aMessage, WPARAM aWParam, LPARAM aLParam);

			bool IsCursorVisible() const { return myIsVisible; }
			void SetCursorVisibility(bool aIsVisible) { myIsVisible = aIsVisible; }

			CursorLockMode GetCursorLockMode() const { return myCursorLockMode; }
			void SetCursorLockMode(CursorLockMode aCursorLockMode) { myCursorLockMode = aCursorLockMode; }

			Vector2i GetPosition() { return myPosition; }
			Vector2f GetRawMouseDelta() { return myRawMouseDelta; }
			Vector2i GetCursorDelta() { return myPositionDelta; }

			bool HasMovedSinceLastUpdate() const;

		private:
			HWND myOwnerWindowHandle { nullptr };
			Vector2i myPosition {};

			Vector2i myPreviousPosition {};
			Vector2i myPositionDelta {};

			Vector2f myRawMouseDelta {};
			Vector2f myNextMouseDelta {};
			Vector2f myOldMouseDelta{};

			std::vector<char> myRawBuffer;

			CursorLockMode myCursorLockMode { CursorLockMode::None };
			bool myIsVisible { true };
	};

}
