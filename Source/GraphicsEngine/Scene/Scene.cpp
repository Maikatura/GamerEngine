#include "GraphicsEngine.pch.h"
#include "Scene.h"

bool Scene::Initialize()
{
	myMainCamera = std::make_shared<Camera>();
	myMainCamera->Initialize(90, { 1920, 1080 }, 0.01f, 1500.0f);

	return true;
}
