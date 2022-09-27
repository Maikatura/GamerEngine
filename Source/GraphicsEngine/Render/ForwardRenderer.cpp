#include "GraphicsEngine.pch.h"
#include "ForwardRenderer.h"
#include <Framework/DX11.h>
#include <Math/Matrix4x4.hpp>
#include <Math/MathTypes.hpp>

#include "Renderer.h"

bool ForwardRenderer::Initialize()
{

	HRESULT result = S_FALSE;
	
	D3D11_BUFFER_DESC bufferDescription = { 0 };
	bufferDescription.Usage = D3D11_USAGE_DYNAMIC;
	bufferDescription.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	bufferDescription.ByteWidth = sizeof(FrameBufferData);
	result = DX11::Device->CreateBuffer(&bufferDescription, nullptr, myFrameBuffer.GetAddressOf());
	if (FAILED(result))
	{
		return false;
	}

	bufferDescription.ByteWidth = sizeof(ObjectBufferData);
	result = DX11::Device->CreateBuffer(&bufferDescription, nullptr, myObjectBuffer.GetAddressOf());
	if (FAILED(result))
	{
		return false;
	}

	bufferDescription.ByteWidth = sizeof(Vector4f);
	result = DX11::Device->CreateBuffer(&bufferDescription, nullptr, myMaterialBuffer.GetAddressOf());
	if(FAILED(result))
	{
		return false;
	}

    return true;
}

void ForwardRenderer::Render(const std::vector<RenderBuffer>& aModelList)
{
	HRESULT result = S_FALSE;
	D3D11_MAPPED_SUBRESOURCE bufferData;



	myFrameBufferData.View = CommonUtilities::Matrix4x4<float>::GetFastInverse(Renderer::GetViewMatrix());
	myFrameBufferData.Projection = Renderer::GetProjectionMatrix();
	
	ZeroMemory(&bufferData, sizeof(D3D11_MAPPED_SUBRESOURCE));
	result = DX11::Context->Map(myFrameBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferData);
	if (FAILED(result)) 
	{
		// BOOM?
		return;
	}

	memcpy(bufferData.pData, &myFrameBufferData, sizeof(FrameBufferData));
	DX11::Context->Unmap(myFrameBuffer.Get(), 0);

	DX11::Context->VSSetConstantBuffers(0, 1, myFrameBuffer.GetAddressOf());
	
	for (const RenderBuffer& model : aModelList)
	{
		if (model.myModel == nullptr)
		{
			return;
		}

		for(int index = 0; index < static_cast<int>(model.myModel->GetNumMeshes()); index++)
		{
			const Model::MeshData& meshData = model.myModel->GetMeshData(index);

			myObjectBufferData.World = model.myTransform;
			ZeroMemory(&bufferData, sizeof(D3D11_MAPPED_SUBRESOURCE));

			myObjectBufferData.myHasBones = false;
			if (model.myModel->GetSkeleton()->GetRoot())
			{
				auto bones = model.myModel->GetBoneTransform();
				myObjectBufferData.myHasBones = true;
				memcpy_s(
					&myObjectBufferData.myBoneData[0], sizeof(CommonUtilities::Matrix4x4<float>) * 128, 
					&bones[0], sizeof(CommonUtilities::Matrix4x4<float>) * 128
				);
			}

			result = DX11::Context->Map(myObjectBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferData);
			if (FAILED(result))
			{
				return;
			}

			memcpy(bufferData.pData, &myObjectBufferData, sizeof(ObjectBufferData));
			DX11::Context->Unmap(myObjectBuffer.Get(), 0);

			if(model.myModel->GetMaterial())
			{
				model.myModel->GetMaterial()->SetAsResource(myMaterialBuffer);
			}
			else
			{
				
			}

			DX11::Context->IASetPrimitiveTopology(static_cast<D3D_PRIMITIVE_TOPOLOGY>(meshData.myPrimitiveTopology));
			DX11::Context->IASetInputLayout(meshData.myInputLayout.Get());
		
			DX11::Context->IASetVertexBuffers(0, 1, meshData.myVertexBuffer.GetAddressOf(), &meshData.myStride, &meshData.myOffset);
			DX11::Context->IASetIndexBuffer(meshData.myIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
		
			DX11::Context->VSSetConstantBuffers(1, 1, myObjectBuffer.GetAddressOf());
			DX11::Context->PSSetConstantBuffers(1, 1, myObjectBuffer.GetAddressOf());

			DX11::Context->VSSetShader(meshData.myVertexShader.Get(), nullptr, 0);
			DX11::Context->PSSetShader(meshData.myPixelShader.Get(), nullptr, 0);
		
			DX11::Context->DrawIndexed(meshData.myNumberOfIndices, 0, 0);
		}
	}
	
}

void ForwardRenderer::Release()
{
	myFrameBuffer->Release();
	myObjectBuffer->Release();
}
