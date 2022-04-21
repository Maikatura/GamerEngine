#include "pch.h"
#include "SceneView.h"
#include <imgui/imgui.h>
#include "Components/MeshComponent.h"
#include "ECS/Entity.h"

namespace GamerEngine
{
	SceneView::SceneView()
		:
		mFrameBuffer(nullptr), myCameraShader(nullptr),
		mLight(nullptr), mSize(800, 600)
	{
		mFrameBuffer = std::make_unique<GamerEngine::OpenGL_FrameBuffer>();
		mFrameBuffer->CreateBuffers(800, 600);
		myCameraShader = std::make_unique<GamerEngine::Shader>();
		myCameraShader->load("shaders/camera.vert", "camera.frag");
		myModelShader = std::make_unique<GamerEngine::Shader>();
		myModelShader->load("shaders/main.vert", "shaders/main.frag");
		mLight = std::make_unique<GamerEngine::Light>();
		myScene = std::make_unique<GamerEngine::Scene>();
	}

	SceneView::~SceneView()
	{
		myCameraShader->unload();
	}


	void SceneView::Resize(int32_t width, int32_t height)
	{
		mSize.x = width;
		mSize.y = height;

		mFrameBuffer->CreateBuffers((int32_t)mSize.x, (int32_t)mSize.y);
	}

	void SceneView::OnMouseMove(double x, double y, GamerEngine::EInputButton button)
	{
		myScene->GetCamera()->SetButton(button);
	}

	/*
	void SceneView::OnMouseWheel(double delta)
	{
		mCamera->OnMouseWheel(delta);
	}

	void SceneView::ResetView()
	{
		mCamera->Reset();
	}
	*/

	void SceneView::LoadMesh(const std::string& filepath)
	{

		auto test = myScene->CreateEntity();
		auto& mesh = test.AddComponent<MeshComponent>();
		mesh.myModel.Load(filepath);
	}


	void SceneView::SetCameraPos(glm::vec3 aPosition)
	{
		myScene->GetCamera()->SetPosition(aPosition);
	}

	void SceneView::Render()
	{

		myCameraShader->use();
		myModelShader->use();

		mLight->Update(myModelShader.get());

		mFrameBuffer->Bind();

		myScene->Render(myModelShader.get());

		/*if (mMesh)
		{
			mMesh->myModel.Update(mShader.get());
			mMesh->myModel.Render();
		}*/


		mFrameBuffer->Unbind();
		ImGui::Begin("Scene");

		

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		mSize = { viewportPanelSize.x, viewportPanelSize.y };


		//std::cout << "X: " << ImGui::GetMousePos().x << " Y: " << ImGui::GetMousePos().y << std::endl;

		

		myScene->RenderCamera(myCameraShader.get());

		//mCamera->SetAspect(mSize.x / mSize.y);
		//mCamera->Update(mMesh.get(), mShader.get());

		// add rendered texture to ImGUI scene window
		uint64_t textureID = mFrameBuffer->GetTexture();
		ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ mSize.x, mSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

		ImGui::End();



	}

	Scene* SceneView::GetScene()
	{
		return myScene.get();
	}
}
