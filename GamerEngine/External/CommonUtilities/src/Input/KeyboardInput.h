#pragma once
#include "Utilites/KeyCodes.h"
#include <bitset>

namespace CommonUtilities
{
	class KeyboardInput
	{
		public:
			KeyboardInput() = default;
			~KeyboardInput() = default;
			bool IsKeyPressed(Key::Code aKeyCode) const;
			bool WasKeyPressedThisFrame(Key::Code aKeyCode) const;
			bool IsKeyReleased(Key::Code aKeyCode) const;
			bool WasKeyReleasedThisFrame(Key::Code aKeyCode) const;
			bool ProcessEvents(uint32_t aMessage, unsigned long long aWParam, long long aLParam);
			void Update();
			void ResetInput();
		private:
			std::bitset<Key::KeysCount> myPreviousKeyboardState {};
			std::bitset<Key::KeysCount> myCurrentKeyboardState {};
			std::bitset<Key::KeysCount> myNextKeyboardState {};
	};
}

