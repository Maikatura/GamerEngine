
#include "GraphicsEngine.pch.h"
#include "EditorCamera.h"

#include "Core/Rendering/Renderer.h"

EditorCamera::EditorCamera()
{
	myCamera = new GamerEngine::CameraComponent();
	myTransform = new GamerEngine::TransformComponent();
	myCameraController = new CameraController();
}

EditorCamera::~EditorCamera()
{
	delete myCamera;
	delete myTransform;
	delete myCameraController;
}

void EditorCamera::Update()
{
	myCamera->BuildTransform(myTransform);
	GamerEngine::Renderer::SetCamera(myCamera, myCamera->GetCurrentViewMatrix(VREye::None), myCamera->GetCurrentViewProjectionMatrix(VREye::None));
	myCameraController->OnUpdate(myCamera, myTransform, Time::GetDeltaTime());
}
