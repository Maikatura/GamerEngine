#include <GraphicsEngine.pch.h>
#include <Scene/Scene.h>

bool Scene::Initialize()
{
	myMainCamera = std::make_shared<Camera>();
	myMainCamera->Initialize(90, { 1920, 1080 }, 0.01f, 50000.0f);
	myMainCamera->SetPosition({ 0, 0, -10.0f });

	return true;
}
