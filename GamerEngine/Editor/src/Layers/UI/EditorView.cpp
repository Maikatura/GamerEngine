#include "Editor.pch.h"
#include "EditorView.h"
#include <DirectXMath.h>
#include <GraphicsEngine.h>
#include <Framework/DX11.h>
#include <Scene/Scene.h>

#include "Components/Components.hpp"
#include "imgui/imgui_internal.h"
#include "ImGuiAdded/ImGuiExtra.h"
#include "Render/Renderer.h"
#include "Render/SelectionData.h"
#include <Model/Entity.h>
#include <Components/CameraController.h>

#include "SettingKeybinds.h"
#include "Debugger/ConsoleHelper.h"
#include "Handlers/DropHandler.h"
#include "Managers/CommandManager.h"
#include "Managers/Commands/PositionCommand.h"
#include "Scene/SceneManager.h"
#include "Layers/NetworkingLayer.h"
#include "Input/Input.h"
#include "Render/DeferredRenderer.h"

EditorView::EditorView() : Layer("Scene"), myStartTranslate(TransformComponent()), myEditedTranslate(TransformComponent())
{

}

void EditorView::OnImGuiRender()
{
	Entity entity = SelectionData::GetEntityObject();

	if(!SceneManager::Get().GetScene())
	{
		return;
	}

	// TODO : Separate to SceneView.h and GameView.h

	//RenderGameView();

	const Ref<Entity> thisEntity = MakeRef<Entity>(entity);

	RenderSceneView(thisEntity);
}

void EditorView::RenderSceneView(const Ref<Entity>& aEntity)
{
	static ImGuiWindowFlags gizmoWindowFlags = 0;
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.f, 0.f));

	BeginMenu(gizmoWindowFlags);
	ImGui::BeginChild("SceneView");

	const ImGuiWindow* window = ImGui::GetCurrentWindow();
	gizmoWindowFlags = ImGui::IsWindowHovered() && ImGui::IsMouseHoveringRect(window->InnerRect.Min, window->InnerRect.Max) ? ImGuiWindowFlags_NoMove : 0;

	CameraController::IsHoveringSceneView = ImGui::IsWindowHovered();

	ImGuizmo::SetDrawlist();
	ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, ImGui::GetWindowWidth(), ImGui::GetWindowHeight());

	ImVec2 windowSize = ImGui::GetWindowSize();

	const auto view = SceneManager::Get().GetScene()->GetRegistry().view<TransformComponent, CameraComponent>();
	for(const auto entity : view)
	{
		auto [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);
		camera.Resize({ static_cast<unsigned int>(windowSize.x), static_cast<unsigned int>(windowSize.y) });
	}


	const int pass = GraphicsEngine::Get()->GetRenderPass();

	ImGui::Image(pass == 0 ? DX11::Get().Get().GetScreenView()->GetShaderResourceView() : GBuffer::GetPasses()[pass - 1].GetShaderResourceView(), {windowSize.x, windowSize.y}); // Use this
	ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowSize.x, windowSize.y);
	//ImGui::Image(GBuffer::GetPasses()[6].GetShaderResourceView(), {windowSize.x, windowSize.y});
	
	//ImGui::Image(GraphicsEngine::Get()->GetScene()->GetDirLight()->GetShadowMapView(), {windowSize.x, windowSize.y});

	RenderEntityParts(aEntity);

	ImGui::EndChild();
	const ImRect rect = ImGui::GetCurrentWindow()->WorkRect;
	DropHandler::DropFileScene(rect, static_cast<ImGuiID>(1231089013290));
	EndMenu();
	ImGui::PopStyleVar();
}

void EditorView::RenderEntityParts(const Ref<Entity>& aEntity)
{
	EditTransform(aEntity);
}

void EditorView::RenderGameView()
{
	//ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.f, 0.f));
	ImGui::Begin(EditorNames::GameViewName.c_str());

	if(ImGui::IsWindowFocused())
	{
		Renderer::SetRenderGame(true);
	}
	else
	{
		Renderer::SetRenderGame(false);
	}

	ImGui::TextWrapped("Nothing");

	ImGui::End();
	//ImGui::PopStyleVar();
}

void EditorView::OnUpdate()
{
	
}

Vector2f EditorView::MouseToViewport(Vector2f aWindowSize, float windowScale)
{
	aWindowSize;
	windowScale;

	if (!ImGui::IsWindowHovered())
	{
		return {0,0};
	}

	std::cout << "Error: Currently Broken, u cant select item in the viewport yet\n";

 	return { 0,0 };
}

void EditorView::EditTransform(const Ref<Entity>& aEntity)
{

	if (!SceneManager::Get().GetScene()->GetRegistry().valid(aEntity->GetHandle()))
	{
		return;
	}

	if (!aEntity->HasComponent<TransformComponent>())
	{
		return;
	}

	auto& transform = aEntity->GetComponent<TransformComponent>();
	Matrix4x4f projectionView = Renderer::GetCamera()->GetHMDMatrixProjectionEye(VREye::None);
	const Matrix4x4f view = Renderer::GetCamera()->GetCurrentViewProjectionMatrix(VREye::None);
	Matrix4x4f viewInverse = Matrix4x4f::GetFastInverse(view);

	Matrix4x4f localMat = transform.GetMatrix();
	float translate[3] = { transform.GetPosition().x, transform.GetPosition().y, transform.GetPosition().z };
	float rotation[3] = { transform.GetRotation().x * CommonUtilities::DegToRad, transform.GetRotation().y * CommonUtilities::DegToRad, transform.GetRotation().z * CommonUtilities::DegToRad };
	float scale[3] = { transform.GetScale().x, transform.GetScale().y, transform.GetScale().z };

	//ImGuizmo::RecomposeMatrixFromComponents(translate, rotation, scale, localMat.Ptr());

	if (!ImGui::IsMouseDown(ImGuiMouseButton_Right))
	{
		if (ImGui::IsKeyPressed(ImGuiKey_W))
		{
			myOperation = ImGuizmo::OPERATION::TRANSLATE;
		}
		if (ImGui::IsKeyPressed(ImGuiKey_E))
		{
			myOperation = ImGuizmo::OPERATION::ROTATE;
		}
		if (ImGui::IsKeyPressed(ImGuiKey_R))
		{
			myOperation = ImGuizmo::OPERATION::SCALE;
		}
	}

	ImGuizmo::Enable(true);

	if (!ImGui::IsMouseDown(ImGuiMouseButton_Right))
	{
		// THIS IS BROKEN AND I DON'T KNOW WHY...
		if (ImGuizmo::Manipulate(viewInverse.Ptr(),projectionView.Ptr(), myOperation, SettingKeybinds::GetEditMode(),localMat.Ptr(),nullptr,nullptr))
		{

			ImGuizmo::DecomposeMatrixToComponents(localMat.Ptr(), translate, rotation, scale);

			transform.SetPosition({ translate[0], translate[1], translate[2] });
			transform.SetRotation(Vector3f{ rotation[0], rotation[1], rotation[2]});
			transform.SetScale({ scale[0], scale[1], scale[2] });
		}
	}


	if (ImGuizmo::IsOver() && ImGuizmo::IsUsing())
	{
		myIsEditingPosition = true;
	}
	else
	{
		myIsEditingPosition = false;
	}

	if (myIsEditingPosition != myOldIsEditingPosition)
	{
		if (myIsFirstTimeEditing)
		{
			myStartTranslate = transform;
			ConsoleHelper::Log(LogType::Info, "Transform Editing Started");
		}
		else
		{
			myEditedTranslate = transform;
			const auto moveCommand = new PositionCommand(aEntity, myStartTranslate, myEditedTranslate);
			CommandManager::DoCommand(moveCommand);
			ConsoleHelper::Log(LogType::Info, "Transform Editing Done");
		}

		myIsFirstTimeEditing = !myIsFirstTimeEditing;
	}

	myOldIsEditingPosition = myIsEditingPosition;
}


