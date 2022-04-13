#pragma once

#include "window/EngineWindow.h"
#include "elems/camera.h"

class Application
{

public:
  Application(const std::string& app_name);

  static Application& Instance() { return *sInstance; }

  void Loop();

private:
  static Application* sInstance;

  std::unique_ptr<GamerEngine::GLWindow> mWindow;
};
