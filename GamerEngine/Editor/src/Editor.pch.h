#pragma once
#ifndef EDITOR_PCH
#define EDITOR_PCH


#ifdef GE_PLATFORM_WINDOWS
	#ifndef NOMINMAX
		// See github.com/skypjack/entt/wiki/Frequently-Asked-Questions#warning-c4003-the-min-the-max-and-the-macro
		#define NOMINMAX
	#endif
#endif


#ifdef GE_PLATFORM_WINDOWS
#include <Windows.h>
#endif

// ImGui Start
#include <ImGui/ImGui.h>
#include <ImGui/backend/imgui_impl_dx11.h>
#include <ImGui/backend/imgui_impl_win32.h>
#include <ImGui/imgui_stdlib.h>
#include "Fonts/IconsFontAwesome.h"
#include <imguizmo/imguizmo.h>
// ImGui End

#include <Math/MathTypes.hpp>
#include <filesystem>

// add headers that you want to pre-compile here



#endif 
