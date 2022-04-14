#pragma once

#include "elems/light.h"
#include "ui/SceneView.h"

#include <imgui/imgui.h>
#include "utils/imgui_widgets.h"
#include <imgui/plugins/ImFileBrowser.h>

namespace GamerEngine
{
	class PropertyPanel
	{
	public:

		PropertyPanel();

		void OnImGuiRender(GamerEngine::SceneView* mScene);

		void SetMeshLoadCallback(const std::function<void(const std::string&)>& callback);

	private:
		ImGui::FileBrowser mFileDialog;
		std::function<void(const std::string&)> mMeshLoadCallback;
		std::string mCurrentFile;


	};
}

