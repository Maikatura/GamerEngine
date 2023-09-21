#include "pch.h"
#include "COMInitializer.h"
#include <Objbase.h>
#include <stdexcept>

COMInitializer::COMInitializer(DWORD coInitFlags)
{
	HRESULT hr = CoInitializeEx(nullptr, coInitFlags);
	if (FAILED(hr))
	{
		// Handle initialization failure
		throw std::runtime_error("COM initialization failed");
	}
}

COMInitializer::~COMInitializer()
{

	CoUninitialize();


}
