// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef GRAPHICSENGINE_PCH
#define GRAPHICSENGINE_PCH

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers


#ifdef GE_PLATFORM_WINDOWS
#ifndef NOMINMAX
	// See github.com/skypjack/entt/wiki/Frequently-Asked-Questions#warning-c4003-the-min-the-max-and-the-macro
#define NOMINMAX
#endif
#endif


#ifdef GE_PLATFORM_WINDOWS
#include <Windows.h>
#endif
#include <d3d11.h>

// Only include in editor
#ifndef EDITOR
#define EDITOR
#include <EngineSettings/Settings.h>
#endif

// Common Utilities
#include <Math/MathTypes.hpp>
#include <Time.hpp>

#include <iostream>
#include <string>
#include <EngineSettingDefines.h>
#include "Managers/ThreadPool.h"
#include <Core/Assert.h>
#include "Utilites/Asserts.h"
#include "Utilites/Pointers.h"

#if _DEBUG
#include <dxgidebug.h>
inline void ReportDX11()
{
	const auto handle = GetModuleHandle(L"dxgidebug.dll");
	if(handle != INVALID_HANDLE_VALUE)
	{
		decltype(&DXGIGetDebugInterface) fun;
		fun = reinterpret_cast<decltype(&DXGIGetDebugInterface)>(GetProcAddress(handle, "DXGIGetDebugInterface"));
		if(fun) // TODO FIXME: "DXGIGetDebugInterface" not found on certain systems
		{
			IDXGIDebug* pDebug = nullptr;
			fun(__uuidof(IDXGIDebug), reinterpret_cast<void**>(&pDebug));
			if(pDebug)
			{
				pDebug->ReportLiveObjects(DXGI_DEBUG_D3D11, DXGI_DEBUG_RLO_ALL);
			}
		}
	}
}
#endif



template<typename T>
inline void SafeRelease(T& aPtr)
{
    if(aPtr != NULL)
    {
        aPtr->Release();
        aPtr = NULL;
    }
}

#endif //GRAPHICSENGINE_PCH
