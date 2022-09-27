#pragma once
#include <Layers/Layer.h>
#include <ImGui/imgui.h>
#include "Imgui/ImGuizmo.h"


class EditorView : public Layer
{
public:

	bool OnImGuiRender() override;

private:
	ImGuizmo::OPERATION myOperation = ImGuizmo::OPERATION::TRANSLATE;
};
