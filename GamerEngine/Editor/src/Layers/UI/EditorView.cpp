#include "Editor.pch.h"
#include "EditorView.h"
#include <DirectXMath.h>
#include <Renderer/GraphicsEngine.h>
#include <Renderer/Framework/DX11.h>
#include <Renderer/Scene/Scene.h>

#include "Components/Components.hpp"
#include "imgui/imgui_internal.h"
#include "ImGuiAdded/ImGuiExtra.h"
#include "Renderer/Render/Renderer.h"
#include "Renderer/Render/SelectionData.h"
#include <Renderer/Model/Entity.h>
#include <Components/CameraController.h>

#include "Renderer/Debugger/ConsoleHelper.h"
#include "Handlers/DropHandler.h"
#include "Renderer/Managers/CommandManager.h"
#include "Renderer/Managers/Commands/PositionCommand.h"
#include "Renderer/Scene/SceneManager.h"
#include "Layers/NetworkingLayer.h"
#include "Layers/Network/MoveObjectMessage.h"

EditorView::EditorView() : Layer("Scene")
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
	RenderSceneView(entity);
}

void EditorView::RenderSceneView(Entity aEntity)
{
	static ImGuiWindowFlags gizmoWindowFlags = 0;
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.f, 0.f));

	BeginMenu(gizmoWindowFlags);
	ImGui::BeginChild("SceneView");

	ImGuiWindow* window = ImGui::GetCurrentWindow();
	gizmoWindowFlags = ImGui::IsWindowHovered() && ImGui::IsMouseHoveringRect(window->InnerRect.Min, window->InnerRect.Max) ? ImGuiWindowFlags_NoMove : 0;

	CameraController::IsHoveringSceneView = ImGui::IsWindowHovered();

	ImGuizmo::SetDrawlist();
	ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, ImGui::GetWindowWidth(), ImGui::GetWindowHeight());

	ImVec2 windowSize = ImGui::GetWindowSize();

	auto view = SceneManager::Get().GetScene()->GetRegistry().view<TransformComponent, CameraComponent>();
	for(auto entity : view)
	{
		auto [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);
		camera.Resize({ static_cast<unsigned int>(windowSize.x), static_cast<unsigned int>(windowSize.y) });
	}

	ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowSize.x, windowSize.y);
	ImGui::Image(DX11::myScreenView->GetShaderResourceView(), { windowSize.x, windowSize.y });
	//ImGui::Image(GraphicsEngine::Get()->GetScene()->GetDirLight()->GetShadowMapView(), {windowSize.x, windowSize.y});

	RenderEntityParts(aEntity);

	ImGui::EndChild();
	ImRect rect = ImGui::GetCurrentWindow()->WorkRect;
	DropHandler::DropFileScene(rect, static_cast<ImGuiID>(1231089013290));
	EndMenu();
	ImGui::PopStyleVar();
}

void EditorView::RenderEntityParts(Entity aEntity)
{
	if(!SceneManager::Get().GetScene()->GetRegistry().valid(aEntity.GetHandle()))
	{
		return;
	}

	if(!aEntity.HasComponent<TransformComponent>())
	{
		return;
	}

	Matrix4x4f projectionView = Renderer::GetCamera()->GetHMDMatrixProjectionEye(VREye::None);
	Matrix4x4f view = Renderer::GetCamera()->GetCurrentViewProjectionMatrix(VREye::None);
	Matrix4x4f viewInverse = Matrix4x4f::GetFastInverse(view);

	DirectX::XMFLOAT4X4 localMat{};
	DirectX::XMFLOAT4X4 viewMat{};
	DirectX::XMFLOAT4X4 projMat{};

	memcpy_s(&viewMat, sizeof(Matrix4x4f), &viewInverse, sizeof(Matrix4x4f));
	memcpy_s(&projMat, sizeof(Matrix4x4f), &projectionView, sizeof(Matrix4x4f));

	TransformComponent& transform = aEntity.GetComponent<TransformComponent>();
	float translate[3] = { transform.Translation.x, transform.Translation.y, transform.Translation.z };
	float rotation[3] = { transform.Rotation.x, transform.Rotation.y, transform.Rotation.z };
	float scale[3] = { transform.Scale.x, transform.Scale.y, transform.Scale.z };

	ImGuizmo::RecomposeMatrixFromComponents(translate, rotation, scale, *localMat.m);

	if(!ImGui::IsMouseDown(ImGuiMouseButton_Right))
	{
		if(ImGui::IsKeyPressed(ImGuiKey_W))
		{
			myOperation = ImGuizmo::OPERATION::TRANSLATE;
		}
		if(ImGui::IsKeyPressed(ImGuiKey_E))
		{
			myOperation = ImGuizmo::OPERATION::ROTATE;
		}
		if(ImGui::IsKeyPressed(ImGuiKey_R))
		{
			myOperation = ImGuizmo::OPERATION::SCALE;
		}
	}

	ImGuizmo::Enable(true);


	ImGuizmo::Manipulate(*viewMat.m,
		*projMat.m,
		myOperation,
		ImGuizmo::WORLD,
		*localMat.m,
		NULL,
		NULL
	);

	if(ImGuizmo::IsOver() && ImGuizmo::IsUsing())
	{
		myIsEditingPosition = true;
	}
	else
	{
		myIsEditingPosition = false;
	}



	ImGuizmo::DecomposeMatrixToComponents(*localMat.m, translate, rotation, scale);

	memcpy_s(&transform.Translation, sizeof(Vector3f), &translate[0], sizeof(Vector3f));
	memcpy_s(&transform.Rotation, sizeof(Vector3f), &rotation[0], sizeof(Vector3f));
	memcpy_s(&transform.Scale, sizeof(Vector3f), &scale[0], sizeof(Vector3f));

	if(myIsEditingPosition != myOldIsEditingPosition)
	{
		if (myIsFirstTimeEditing)
		{
			myStartTranslate = transform;
			ConsoleHelper::Log(LogType::Info, "Transform Editing Started");
		}
		else
		{
			myEditedTranslate = transform;
			auto moveCommand = new PositionCommand(aEntity, myStartTranslate, myEditedTranslate);
			CommandManager::DoCommand(moveCommand);
			ConsoleHelper::Log(LogType::Info, "Transform Editing Done");

			TurNet::TurMessage outMsg;
			ObjectMoveMessage moveMsg;
			moveMsg.EntityID = aEntity.GetComponent<IDComponent>().ID;
			moveMsg.Transform = transform;
		
			outMsg << moveMsg;

			NetworkingLayer::GetClient().SendToServer(outMsg);
		}

		myIsFirstTimeEditing = !myIsFirstTimeEditing;
	}

	myOldIsEditingPosition = myIsEditingPosition;
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


