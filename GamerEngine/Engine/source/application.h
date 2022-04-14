#pragma once

#include "window/EngineWindow.h"
#include "elems/camera.h"

class Application
{

public:
	Application();

	static Application& Instance() { return *sInstance; }

	void Init(const std::string& aAppName, int aMajorVersion, int aMinorVersion, int aPatchVersion);
	void Loop();


private:
	static Application* sInstance;

	std::unique_ptr<GamerEngine::GLWindow> mWindow;
};
