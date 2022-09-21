#include "EditorView.h"
#include <DirectXMath.h>
#include <GraphicsEngine.h>
#include <Framework/DX11.h>
#include <ImGui/imgui.h>
#include <Imgui/ImGuizmo.h>
#include <Scene/Scene.h>


bool EditorView::OnImGuiRender()
{
	std::shared_ptr<SelectedObejct> selectedObjectData = GraphicsEngine::Get()->GetScene()->GetSelectedObject();
	ModelInstance* selectedObject = selectedObjectData->GetType<ModelInstance>();

	if (selectedObjectData->FileType != FileType::Model)
	{
		return true;
	}

	auto camera = GraphicsEngine::Get()->GetScene()->GetCamera();
	auto cameraView = camera->GetTransform();
	auto projectionView = camera->GetProjectionMatrixMutable();


	auto test = Matrix4x4f::GetFastInverse(cameraView.GetMatrix());

	DirectX::XMFLOAT4X4 localMat;
	DirectX::XMFLOAT4X4 viewMat;
	DirectX::XMFLOAT4X4 projMat;
	DirectX::XMStoreFloat4x4(&localMat, *(DirectX::FXMMATRIX*)selectedObject->GetTransform().GetMatrix().GetFloatPtr().data());
	DirectX::XMStoreFloat4x4(&viewMat, *(DirectX::FXMMATRIX*)test.GetFloatPtr().data());
	DirectX::XMStoreFloat4x4(&projMat, *(DirectX::FXMMATRIX*)camera->GetProjectionMatrixMutable().GetFloatPtr().data());



	float translate[3] = { 0.0f, 0.0f, 0.0f };
	float rotation[3] = { 0.0f, 0.0f, 0.0f };
	float scale[3] = { 0.0f, 0.0f, 0.0f };

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
	ImGuizmo::SetRect(clientRect.left, clientRect.top, io.DisplaySize.x, io.DisplaySize.y);

	if (selectedObject->GetModel() != nullptr)
	{
		ImGuizmo::Manipulate(*viewMat.m,
			*projMat.m,
			myOperation,
			ImGuizmo::LOCAL,
			*localMat.m, NULL, NULL);
	}

	/*ImGuizmo::DrawGrid(*viewMat.m,
		*projMat.m, CommonUtilities::Matrix4x4<float>().GetFloatPtr().data(), 100.0f);*/

	ImGuizmo::DecomposeMatrixToComponents(*localMat.m, translate, rotation, scale);

	selectedObject->GetTransform().ComposeTransform(
		{ translate[0], translate[1], translate[2] },
		{ rotation[0], rotation[1], rotation[2] },
		{ scale[0], scale[1], scale[2] }
	);


	return true;
}
