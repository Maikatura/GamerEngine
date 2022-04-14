#include "pch.h"
#include "PropertyPanel.h"

namespace GamerEngine
{
	PropertyPanel::PropertyPanel()
	{
		mCurrentFile = "< ... >";
		mFileDialog.SetTitle("Open mesh");
		mFileDialog.SetFileFilters({ ".fbx", ".obj" });
	}

	void PropertyPanel::OnImGuiRender(GamerEngine::SceneView* scene_view)
	{
		auto mesh = scene_view->GetMesh();

		ImGui::Begin("Properties");
		if (ImGui::CollapsingHeader("Mesh", ImGuiTreeNodeFlags_DefaultOpen))
		{

			if (ImGui::Button("Open..."))
			{
				mFileDialog.Open();
			}
			ImGui::SameLine(0, 5.0f);
			ImGui::Text(mCurrentFile.c_str());
		}

		if (ImGui::CollapsingHeader("Material") && mesh)
		{
			ImGui::ColorPicker3("Color", (float*)&mesh->myModel.mColor, ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_DisplayRGB);
			ImGui::SliderFloat("Roughness", &mesh->myModel.mRoughness, 0.0f, 1.0f);
			ImGui::SliderFloat("Metallic", &mesh->myModel.mMetallic, 0.0f, 1.0f);
		}

		if (ImGui::CollapsingHeader("Light"))
		{

			ImGui::Separator();
			ImGui::Text("Position");
			ImGui::Separator();
			GamerEngine::draw_vec3_widget("Position", scene_view->get_light()->mPosition, 80.0f);
		}

		ImGui::End();

		mFileDialog.Display();
		if (mFileDialog.HasSelected())
		{
			auto file_path = mFileDialog.GetSelected().string();
			mCurrentFile = file_path.substr(file_path.find_last_of("/\\") + 1);

			mMeshLoadCallback(file_path);

			mFileDialog.ClearSelected();
		}

	}

	void PropertyPanel::SetMeshLoadCallback(const std::function<void(const std::string&)>& callback)
	{
		mMeshLoadCallback = callback;
	}
}
