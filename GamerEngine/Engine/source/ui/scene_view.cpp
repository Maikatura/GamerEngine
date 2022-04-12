#include "pch.h"
#include "scene_view.h"
#include "property_panel.h"
#include "imgui.h"

namespace nui
{
	void SceneView::Resize(int32_t width, int32_t height)
	{
		mSize.x = width;
		mSize.y = height;

		mFrameBuffer->create_buffers((int32_t)mSize.x, (int32_t)mSize.y);
	}

	void SceneView::on_mouse_move(double x, double y, nelems::EInputButton button)
	{
		mCamera->on_mouse_move(x, y, button);
	}

	void SceneView::on_mouse_wheel(double delta)
	{
		mCamera->on_mouse_wheel(delta);
	}

	void SceneView::load_mesh(const std::string& filepath)
	{
		if (!mMesh)
			mMesh = std::make_shared<Model>();

		mMesh->Load(filepath);


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

		if (mMesh)
		{
			ImGui::Begin("Inspector");

			ImGui::Text("Transform");
			ImGui::Text("Position");
			nimgui::DrawImGuiPosition("Position", mMesh->myTransform);
			ImGui::Text("Rotation");
			nimgui::DrawImGuiRotation("Rotation", mMesh->myTransform);
			ImGui::Text("Scale");
			nimgui::DrawImGuiScale("Scale", mMesh->myTransform);

			ImGui::End();
		}
		ImGui::Begin("Scene");

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		mSize = { viewportPanelSize.x, viewportPanelSize.y };

		mCamera->set_aspect(mSize.x / mSize.y);
		mCamera->update(mShader.get());

		// add rendered texture to ImGUI scene window
		uint64_t textureID = mFrameBuffer->get_texture();
		ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ mSize.x, mSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

		ImGui::End();

	}
}
