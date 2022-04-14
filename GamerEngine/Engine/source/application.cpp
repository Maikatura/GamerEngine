#include "pch.h"

#include "Application.h"

#include "window/EngineWindow.h"

Application::Application()
{
	
	
}

void Application::Init(const std::string& aAppName, int aMajorVersion, int aMinorVersion, int aPatchVersion)
{
	std::cout << "Starting " + aAppName;
	std::cout << " | Version: " + std::to_string(aMajorVersion) + "." + std::to_string(aMinorVersion) + "." + std::to_string(aPatchVersion) << std::endl;
	std::cout << "Never stop gaming!" << std::endl;

	std::string fullWindowName;
	fullWindowName += aAppName;
	fullWindowName += " ";
	fullWindowName += "Version:";
	fullWindowName += " ";
	fullWindowName += std::to_string(aMajorVersion);
	fullWindowName += ".";
	fullWindowName += std::to_string(aMinorVersion);
	fullWindowName += ".";
	fullWindowName += std::to_string(aPatchVersion);
	fullWindowName += " ";
#if _DEBUG
	fullWindowName += "[Debug]"; 
#else
	fullWindowName += "[Release]";
#endif


	mWindow = std::make_unique<GamerEngine::GLWindow>();
	mWindow->Init(1280, 720, fullWindowName);
}

void Application::Loop()
{
	while (mWindow->IsRunning())
	{
		mWindow->Render();
	}
}
