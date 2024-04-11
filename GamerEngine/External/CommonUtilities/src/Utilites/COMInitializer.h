#pragma once
#include <Objbase.h>

class COMInitializer
{
public:
    COMInitializer(DWORD coInitFlags = COINIT_MULTITHREADED);

    ~COMInitializer();

    // Disable copy and move constructors and assignments
    COMInitializer(const COMInitializer&) = delete;
    COMInitializer& operator=(const COMInitializer&) = delete;
    COMInitializer(COMInitializer&&) = delete;
    COMInitializer& operator=(COMInitializer&&) = delete;
};
