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
		mFrameBuffer->create_buffers(800, 600);
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

		mFrameBuffer->create_buffers((int32_t)mSize.x, (int32_t)mSize.y);
	}

	void SceneView::OnMouseMove(double x, double y, GamerEngine::EInputButton button)
	{
		mCamera->on_mouse_move(x, y, button);
	}

	void SceneView::OnMouseWheel(double delta)
	{
		mCamera->on_mouse_wheel(delta);
	}

	void SceneView::ResetView()
	{
		mCamera->reset();
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

	void SceneView::Render()
	{

		mShader->use();

		mLight->update(mShader.get());

		mFrameBuffer->bind();

		if (mMesh)
		{
			mMesh->update(mShader.get());
			mMesh->Render();
		}

		mFrameBuffer->unbind();
		ImGui::Begin("Scene");

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		mSize = { viewportPanelSize.x, viewportPanelSize.y };

		mCamera->set_aspect(mSize.x / mSize.y);
		mCamera->update(mMesh.get(), mShader.get());

		// add rendered texture to ImGUI scene window
		uint64_t textureID = mFrameBuffer->get_texture();
		ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ mSize.x, mSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

		ImGui::End();

	}
}
