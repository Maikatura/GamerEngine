// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef GRAPHICSENGINE_PCH
#define GRAPHICSENGINE_PCH

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers


// add headers that you want to pre-compile here
#include <Windows.h>
#include <d3d11.h>

// Only include in editor
#ifndef EDITOR
#define EDITOR
#include <ImGui/imgui.h>
#include <EngineSettings/Settings.h>


#endif

// Common Utilities
#include <Math/MathTypes.hpp>
#include <Time.hpp>

#include <iostream>
#include <string>




#if _DEBUG
#include <dxgidebug.h>

inline void ReportDX11()
{
	auto handle = GetModuleHandle(L"dxgidebug.dll");
	if(handle != INVALID_HANDLE_VALUE)
	{
		auto fun = reinterpret_cast<decltype(&DXGIGetDebugInterface)>(GetProcAddress(handle, "DXGIGetDebugInterface"));
		if(fun) // TODO FIXME: "DXGIGetDebugInterface" not found on certain systems
		{
			IDXGIDebug* pDebug = nullptr;
			fun(__uuidof(IDXGIDebug), (void**)&pDebug);
			if(pDebug)
			{
				pDebug->ReportLiveObjects(DXGI_DEBUG_D3D11, DXGI_DEBUG_RLO_ALL);
			}
		}
	}
}

#endif


template<typename T>
inline void SafeRelease(T& ptr)
{
    if(ptr != NULL)
    {
        ptr->Release();
        ptr = NULL;
    }
}

#endif //GRAPHICSENGINE_PCH
