#pragma once
#include <bitset>
#include "COMInitializer.h"
#include "KeyCodes.h"

namespace CommonUtilities
{

	extern Key::Code WindowsVKToCUKey(int aVK, WORD aLParamHiWord);

	extern Mouse::Button XButtonFromWParam(WPARAM aWParam);

	extern std::bitset<Xbox::ButtonsCount> XInputToCUButtons(WORD aXInputButtons);
}
