#include "EditorView.h"
#include <DirectXMath.h>
#include <GraphicsEngine.h>
#include <Framework/DX11.h>
#include <ImGui/imgui.h>
#include <Imgui/ImGuizmo.h>
#include <Scene/Scene.h>

#include "Components/Components.hpp"
#include "Input/Input.h"
#include "Model/Entity.h"
#include "Render/Renderer.h"
#include "Render/SelectionData.h"
#include "Scene/SceneSerializer.h"


bool EditorView::OnImGuiRender()
{
	auto selectedObjectData = SelectionData::GetSelectedObject();
	auto entityPtr = reinterpret_cast<uint32_t>(selectedObjectData->GetType<entt::entity>());
	Entity entity{ static_cast<entt::entity>(entityPtr),  GraphicsEngine::Get()->GetScene().get() };


	if (Input::IsKeyDown(VK_CONTROL) && Input::IsKeyPressed('S'))
	{
		std::cout << "Saved scene" << std::endl;

		SceneSerializer saveScene(GraphicsEngine::Get()->GetScene().get());
		saveScene.Serialize(GraphicsEngine::Get()->GetScene()->GetPath());
	}

	/*
	ImGui::Begin("Scene");
	auto windowSize = ImGui::GetWindowSize();
	ImGui::Image(DX11::RenderSRV.Get(), { windowSize.x, windowSize.y });
	ImGui::End();
	*/

	if (selectedObjectData->FileType != FileType::SceneObject)
	{
		return true;
	}

	if (!GraphicsEngine::Get()->GetScene()->GetRegistry().valid(entity))
	{
		return true;
	}

	if(!entity.HasComponent<TransformComponent>())
	{
		return true;
	}

	const auto cameraView = Renderer::GetViewMatrix();
	Matrix4x4f projectionView = Renderer::GetProjectionMatrix();


	auto test = Matrix4x4f::GetFastInverse(cameraView);

	DirectX::XMFLOAT4X4 localMat{};
	DirectX::XMFLOAT4X4 viewMat{};
	DirectX::XMFLOAT4X4 projMat{};

	

	DirectX::XMStoreFloat4x4(&viewMat, *(DirectX::FXMMATRIX*)test.GetFloatPtr().data());
	DirectX::XMStoreFloat4x4(&projMat, *(DirectX::FXMMATRIX*)projectionView.GetFloatPtr().data());

	auto& transform = entity.GetComponent<TransformComponent>();
	float translate[3] = { transform.Translation.x, transform.Translation.y, transform.Translation.z};
	float rotation[3] = { transform.Rotation.x, transform.Rotation.y, transform.Rotation.z };
	float scale[3] = { transform.Scale.x, transform.Scale.y, transform.Scale.z };

	ImGuizmo::RecomposeMatrixFromComponents(translate, rotation, scale, *localMat.m);

	if (!ImGui::IsMouseDown(ImGuiMouseButton_Right))
	{
		if(ImGui::IsKeyPressed('Q'))
		{
			myOperation = ImGuizmo::OPERATION::TRANSLATE;
		}
		if(ImGui::IsKeyPressed('W'))
		{
			myOperation = ImGuizmo::OPERATION::ROTATE;
		}
		if(ImGui::IsKeyPressed('E'))
		{
			myOperation = ImGuizmo::OPERATION::SCALE;
		}
	}
	ImGuiIO& io = ImGui::GetIO();
	ImGuizmo::Enable(true);

	RECT clientRect = { 0,0,0,0 };
	GetWindowRect(GraphicsEngine::Get()->GetWindowHandle(), &clientRect);
	ImGuizmo::SetRect(static_cast<float>(clientRect.left), static_cast<float>(clientRect.top), io.DisplaySize.x, io.DisplaySize.y);

	if (!entity.HasComponent<ModelComponent>())
	{
		return true;
	}

	auto& model = entity.GetComponent<ModelComponent>();

		ImGuizmo::Manipulate(*viewMat.m,
			*projMat.m,
			myOperation,
			ImGuizmo::LOCAL,
			*localMat.m, NULL, NULL);
	

	/*ImGuizmo::DrawGrid(*viewMat.m,
		*projMat.m, CommonUtilities::Matrix4x4<float>().GetFloatPtr().data(), 100.0f);*/

	ImGuizmo::DecomposeMatrixToComponents(*localMat.m, translate, rotation, scale);

	transform.Translation = { translate[0], translate[1], translate[2] };
	transform.Rotation = { rotation[0], rotation[1], rotation[2] };
	transform.Scale = { scale[0], scale[1], scale[2] };

	return true;
}
