#pragma once
#ifndef EDITOR_PCH
#define EDITOR_PCH

#define WIN32_LEAN_AND_MEAN
#ifdef WIN32_LEAN_AND_MEAN            // Exclude rarely-used stuff from Windows headers
#include <windows.h>
#include <Utilites/StringCast.h>
#endif
// ImGui Start
#include <ImGui/ImGui.h>
#include <ImGui/imgui_impl_dx11.h>
#include <ImGui/imgui_impl_win32.h>
#include <ImGui/imgui_stdlib.h>
#include "Fonts/IconsForkAwesome.h"
#include <imguizmo/imguizmo.h>
// ImGui End

#include <Math/MathTypes.hpp>
#include <filesystem>

// add headers that you want to pre-compile here



#endif 
