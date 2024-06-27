#include "Editor.pch.h"
#include "SceneView.h"
#include <DirectXMath.h>
#include <GraphicsEngine.h>
#include <Core/Framework/DX11.h>
#include <Scene/Scene.h>
#include "imgui/imgui_internal.h"
#include "ImGuiAdded/ImGuiExtra.h"
#include "Core/Rendering/Renderer.h"
#include "Core/Rendering/SelectionData.h"
#include <Core/Model/Entity.h>
#include <Components/CameraController.h>

#include "SettingKeybinds.h"
#include "Debugger/ConsoleHelper.h"
#include "Handlers/DropHandler.h"
#include "Managers/CommandManager.h"
#include "Managers/Commands/PositionCommand.h"
#include "Scene/SceneManager.h"
#include "Layers/NetworkingLayer.h"
#include "Core/Rendering/DeferredRenderer.h"

SceneView::SceneView() : Layer("Scene"), myStartTranslate(GamerEngine::Transform()), myEditedTranslate(GamerEngine::Transform())
{

}

void SceneView::OnImGuiRender()
{
	GamerEngine::Entity entity = SelectionData::GetEntityObject();

	if(!SceneManager::Get().GetScene())
	{
		return;
	}

	// TODO : Separate to SceneView.h and GameView.h

	//RenderGameView();

	const Ref<GamerEngine::Entity> thisEntity = MakeRef<GamerEngine::Entity>(entity);

	RenderSceneView(thisEntity);
}

void SceneView::RenderSceneView(const Ref<GamerEngine::Entity>& aEntity)
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

	SceneManager::Get().GetScene()->Resize({ static_cast<unsigned int>(windowSize.x), static_cast<unsigned int>(windowSize.y) });


	ImGui::Image(DX11::Get().Get().GetScreenView()->GetShaderResourceView(), { windowSize.x, windowSize.y }); // Use this
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

void SceneView::RenderEntityParts(const Ref<GamerEngine::Entity>& aEntity)
{
	EditTransform(aEntity);
}

void SceneView::RenderGameView()
{
	//ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.f, 0.f));
	ImGui::Begin(EditorNames::GameViewName.c_str());

	if(ImGui::IsWindowFocused())
	{
		GamerEngine::Renderer::SetRenderGame(true);
	}
	else
	{
		GamerEngine::Renderer::SetRenderGame(false);
	}

	ImGui::TextWrapped("Nothing");

	ImGui::End();
	//ImGui::PopStyleVar();
}

void SceneView::OnUpdate()
{
	
}

Vector2f SceneView::MouseToViewport(Vector2f aWindowSize, float windowScale)
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

void SceneView::EditTransform(const Ref<GamerEngine::Entity>& aEntity)
{

	if (!SceneManager::Get().GetScene()->GetRegistry().valid(aEntity->GetHandle()))
	{
		return;
	}

	if (!aEntity->HasComponent<GamerEngine::TransformComponent>())
	{
		return;
	}

	if (!GamerEngine::Renderer::GetCamera())
	{
		return;
	}

	auto transform = aEntity->GetComponent<GamerEngine::TransformComponent>().GetWorldTransform();
	Matrix4x4f projectionView = GamerEngine::Renderer::GetProjectionMatrix();
	const Matrix4x4f view = GamerEngine::Renderer::GetViewMatrix();
	Matrix4x4f viewInverse = Matrix4x4f::GetFastInverse(view);

	Matrix4x4f localMat; // = transform.GetMatrix();
	float translate[3] = { transform.Translation.x, transform.Translation.y, transform.Translation.z };
	float rotation[3] = { transform.Rotation.x, transform.Rotation.y, transform.Rotation.z };
	float scale[3] = { transform.Scale.x, transform.Scale.y, transform.Scale.z };

	ImGuizmo::RecomposeMatrixFromComponents(translate, rotation, scale, localMat.Ptr());

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

			aEntity->GetComponent<GamerEngine::TransformComponent>().SetPosition({ translate[0], translate[1], translate[2] });
			aEntity->GetComponent<GamerEngine::TransformComponent>().SetRotation(Vector3f{ rotation[0], rotation[1], rotation[2] });
			aEntity->GetComponent<GamerEngine::TransformComponent>().SetScale({ scale[0], scale[1], scale[2] });
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


