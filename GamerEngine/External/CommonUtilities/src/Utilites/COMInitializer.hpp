#pragma once
#include <windows.h>        // For Windows API functions
#include <stdexcept>        // For std::runtime_error
#include <iostream>         // For error handling and output

class COMInitializer {
public:
    COMInitializer(DWORD coInitFlags = COINIT_MULTITHREADED)
	{
        HRESULT hr = CoInitializeEx(nullptr, coInitFlags);
        if (FAILED(hr)) 
        {
            // Handle initialization failure
            throw std::runtime_error("COM initialization failed");
        }
    }

    ~COMInitializer()
	{
        CoUninitialize();
    }

    // Disable copy and move constructors and assignments
    COMInitializer(const COMInitializer&) = delete;
    COMInitializer& operator=(const COMInitializer&) = delete;
    COMInitializer(COMInitializer&&) = delete;
    COMInitializer& operator=(COMInitializer&&) = delete;
};
