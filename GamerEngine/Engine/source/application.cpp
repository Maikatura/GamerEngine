#include "pch.h"

#include "Application.h"

#include "window/Engine_window.h"

Application::Application(const std::string& app_name)
{
	mWindow = std::make_unique<nwindow::GLWindow>();
	mWindow->Init(1280, 720, app_name);
}

void Application::Loop()
{
	while (mWindow->IsRunning())
	{
		mWindow->Render();
	}
}