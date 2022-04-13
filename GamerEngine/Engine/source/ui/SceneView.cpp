#include "pch.h"
#include "SceneView.h"
#include "PropertyPanel.h"
#include "imgui.h"

namespace GamerEngine
{
	SceneView::SceneView()
		:
		mCamera(nullptr), mFrameBuffer(nullptr), mShader(nullptr),
		mLight(nullptr), mSize(800, 600)
	{
		mFrameBuffer = std::make_unique<GamerEngine::OpenGL_FrameBuffer>();
		mFrameBuffer->CreateBuffers(800, 600);
		mShader = std::make_unique<nshaders::Shader>();
		mShader->load("shaders/vs.shader", "shaders/fs_pbr.shader");
		mLight = std::make_unique<GamerEngine::Light>();

		mCamera = std::make_unique<GamerEngine::Camera>(glm::vec3(0, 0, 3), 45.0f, 1.3f, 0.1f, 1000.0f);

	}

	SceneView::~SceneView()
	{

		mShader->unload();

	}


	void SceneView::Resize(int32_t width, int32_t height)
	{
		mSize.x = width;
		mSize.y = height;

		mFrameBuffer->CreateBuffers((int32_t)mSize.x, (int32_t)mSize.y);
	}

	void SceneView::OnMouseMove(double x, double y, GamerEngine::EInputButton button)
	{
		mCamera->OnMouseMove(x, y, button);
	}

	void SceneView::OnMouseWheel(double delta)
	{
		mCamera->OnMouseWheel(delta);
	}

	void SceneView::ResetView()
	{
		mCamera->Reset();
	}

	void SceneView::LoadMesh(const std::string& filepath)
	{
		if (!mMesh)
		{
			mMesh = std::make_shared<Model>();
		}

		mMesh->Load(filepath);
	}

	void SceneView::SetMesh(std::shared_ptr<Model> mesh)
	{
		mMesh = mesh;
	}

	void SceneView::SetCameraPos(glm::vec3 aPosition)
	{
		mCamera->SetPosition(aPosition);
	}

	void SceneView::Render()
	{

		mShader->use();

		mLight->Update(mShader.get());

		mFrameBuffer->Bind();

		if (mMesh)
		{
			mMesh->Update(mShader.get());
			mMesh->Render();
		}

		mFrameBuffer->Unbind();
		ImGui::Begin("Scene");

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		mSize = { viewportPanelSize.x, viewportPanelSize.y };

		mCamera->SetAspect(mSize.x / mSize.y);
		mCamera->Update(mMesh.get(), mShader.get());

		// add rendered texture to ImGUI scene window
		uint64_t textureID = mFrameBuffer->GetTexture();
		ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ mSize.x, mSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

		ImGui::End();

	}
}
