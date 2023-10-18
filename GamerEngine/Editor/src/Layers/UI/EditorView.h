#pragma once
#include <Layers/Layer.h>
#include "ImGuizmo/ImGuizmo.h"

#include "Components/Components.hpp"
#include "Math/MathTypes.hpp"

class Entity;

class EditorView : public Layer
{
public:

	EditorView();


	void OnImGuiRender() override;

	void RenderSceneView(Entity aEntity);
	void RenderGameView();

	void OnUpdate() override;

private:

	Vector2f MouseToViewport(Vector2f aWindowSize, float windowScale);

	void RenderEntityParts(Entity aEntity);

	int myTabOpen = 0;
	int myRenderPass = 0;

	bool myIsFirstTimeEditing = true;
	bool myIsEditingPosition = false;
	bool myOldIsEditingPosition = false;

	TransformComponent myStartTranslate;
	TransformComponent myEditedTranslate;

	ImGuizmo::OPERATION myOperation = ImGuizmo::OPERATION::TRANSLATE;
};
