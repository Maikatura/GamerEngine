#pragma once
#include <Layers/Layer.h>

#include "ImGuizmo/GraphEditor.h"
#include "Math/MathTypes.hpp"


struct Node
{
	std::string name;
	GraphEditor::TemplateIndex templateIndex;
	float x, y;
	bool mSelected;
};

class AnimatorLayer : public Layer
{
public:

	AnimatorLayer();

	bool OnImGuiRender() override;
};