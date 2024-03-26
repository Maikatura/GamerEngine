#pragma once
#include <Layers/Layer.h>

#include "Components/Camera.h"
#include "ImGuizmo/ImGuizmo.h"

#include "Components/TransfromComponent.h"
#include "Math/MathTypes.hpp"
#include "Utilites/Pointers.h"

namespace ImGuizmo
{
	struct matrix_t;
}

class Entity;

class EditorView : public Layer
{
public:

	EditorView();


	void OnImGuiRender() override;

	void RenderSceneView(const Ref<Entity>& aEntity);
	void RenderGameView();

	void OnUpdate() override;

private:

	Vector2f MouseToViewport(Vector2f aWindowSize, float windowScale);

	void EditTransform(const Ref<Entity>& aEntity);

	void RenderEntityParts(const Ref<Entity>& aEntity);

	int myTabOpen = 0;
	int myRenderPass = 0;

	bool myIsFirstTimeEditing = true;
	bool myIsEditingPosition = false;
	bool myOldIsEditingPosition = false;

	TransformComponent myStartTranslate;
	TransformComponent myEditedTranslate;

	ImGuizmo::OPERATION myOperation = ImGuizmo::OPERATION::TRANSLATE;
};
