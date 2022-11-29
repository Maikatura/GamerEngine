#include "GraphicsEngine.pch.h"
#include "ForwardRenderer.h"
#include <Framework/DX11.h>
#include "GraphicsEngine.h"
#include "Renderer.h"
#include "Light/DirectionalLight.h"
#include "Light/EnvironmentLight.h"
#include "Sort/Sort.hpp"
#include <Particles/particleemitter.h>

#include "AssetHandlers/ModelAssetHandler.h"

bool ForwardRenderer::Initialize()
{

	HRESULT result = S_FALSE;

	D3D11_BUFFER_DESC bufferDescription = { 0 };
	bufferDescription.Usage = D3D11_USAGE_DYNAMIC;
	bufferDescription.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	bufferDescription.ByteWidth = sizeof(FrameBufferData);
	result = DX11::Device->CreateBuffer(&bufferDescription, nullptr, myFrameBuffer.GetAddressOf());
	if(FAILED(result))
	{
		return false;
	}

	bufferDescription.ByteWidth = sizeof(ObjectBufferData);
	result = DX11::Device->CreateBuffer(&bufferDescription, nullptr, myObjectBuffer.GetAddressOf());
	if(FAILED(result))
	{
		return false;
	}

	bufferDescription.ByteWidth = sizeof(Vector4f);
	result = DX11::Device->CreateBuffer(&bufferDescription, nullptr, myMaterialBuffer.GetAddressOf());
	if(FAILED(result))
	{
		return false;
	}

	bufferDescription.ByteWidth = sizeof(SceneLightBuffer);
	result = DX11::Device->CreateBuffer(&bufferDescription, nullptr, myLightBuffer.GetAddressOf());
	if(FAILED(result))
	{
		return false;
	}


	BuildDepth();

	return true;
}

void ForwardRenderer::Render(const std::vector<RenderBuffer>& aModelList, const std::shared_ptr<DirectionalLight>& aDirectionalLight, const std::shared_ptr<EnvironmentLight>& anEnvironmentLight, const std::vector<Light*>& aLightList)
{
	if(aModelList.empty())
	{
		return;
	}

	HRESULT result = S_FALSE;
	D3D11_MAPPED_SUBRESOURCE bufferData;

	myFrameBufferData.View = Matrix4x4f::GetFastInverse(Renderer::GetViewMatrix());
	myFrameBufferData.CamTranslation = Renderer::GetViewMatrix().GetPosition();
	myFrameBufferData.Projection = Renderer::GetProjectionMatrix();
	myFrameBufferData.RenderMode = static_cast<int>(GraphicsEngine::Get()->GetRenderMode());

	RECT clientRect = DX11::GetClientSize();
	const Vector2ui Resolution = {
				static_cast<unsigned int>(clientRect.right - clientRect.left),
				static_cast<unsigned int>(clientRect.bottom - clientRect.top)
	};
	myFrameBufferData.Resolution = Resolution;

	ZeroMemory(&bufferData, sizeof(D3D11_MAPPED_SUBRESOURCE));
	result = DX11::Context->Map(myFrameBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferData);
	if(FAILED(result))
	{
		// BOOM?
		return;
	}

	memcpy(bufferData.pData, &myFrameBufferData, sizeof(FrameBufferData));
	DX11::Context->Unmap(myFrameBuffer.Get(), 0);

	DX11::Context->VSSetConstantBuffers(0, 1, myFrameBuffer.GetAddressOf());

	SetBlendState(BlendState::AlphaBlend);
	SetDepthStencilState(DepthStencilState::DSS_ReadWrite);

	if(aDirectionalLight)
	{
		mySceneLightBufferData.DirectionalLight = aDirectionalLight->GetLightBufferData();
		aDirectionalLight->SetAsResource();
	}

	if(anEnvironmentLight)
	{
		anEnvironmentLight->SetAsResource(nullptr);
	}

	mySceneLightBufferData.NumLights = 0;
	ZeroMemory(mySceneLightBufferData.Lights, sizeof(Light::LightBufferData) * MAX_FORWARD_LIGHTS);
	for(size_t l = 0; l < aLightList.size() && l < MAX_FORWARD_LIGHTS; l++)
	{
		mySceneLightBufferData.Lights[l] = aLightList[l]->GetLightBufferData();
		mySceneLightBufferData.NumLights++;
		aLightList[l]->SetAsResource(nullptr);
	}


	result = DX11::Context->Map(
		myLightBuffer.Get(),
		0,
		D3D11_MAP_WRITE_DISCARD,
		0,
		&bufferData);

	if(FAILED(result))
	{
		// NOOOOOOOOOOOOOO :(
	}

	memcpy(bufferData.pData, &mySceneLightBufferData, sizeof(SceneLightBuffer));

	DX11::Context->Unmap(myLightBuffer.Get(), 0);
	DX11::Context->PSSetConstantBuffers(3, 1, myLightBuffer.GetAddressOf());

	ModelAssetHandler::ResetRenderedModels();

	for(const RenderBuffer& model : aModelList)
	{
		if(model.myModel == nullptr)
		{
			return;
		}

		if (model.myModel->RenderWithDeferred())
		{
			return;
		}

		std::shared_ptr<ModelInstance> modelInstance = model.myModel;

		bool isInstanced = modelInstance->HasRenderedInstance();

		myObjectBufferData.IsInstanced = isInstanced;
		myObjectBufferData.World = model.myTransform;
		myObjectBufferData.WorldInv = Matrix4x4f::GetFastInverse(model.myTransform);
		ZeroMemory(&bufferData, sizeof(D3D11_MAPPED_SUBRESOURCE));

		myObjectBufferData.myHasBones = false;
		if(model.myModel->GetSkeleton()->GetRoot())
		{
			auto bones = modelInstance->GetBoneTransform();
			myObjectBufferData.myHasBones = true;
			memcpy_s(
				&myObjectBufferData.BoneData[0], sizeof(Matrix4x4f) * MAX_MODEL_BONES,
				&bones[0], sizeof(Matrix4x4f) * MAX_MODEL_BONES
			);
		}

		result = DX11::Context->Map(myObjectBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferData);
		if(FAILED(result))
		{
			// WTF
		}

		memcpy(bufferData.pData, &myObjectBufferData, sizeof(ObjectBufferData));
		DX11::Context->Unmap(myObjectBuffer.Get(), 0);



		for(int index = 0; index < modelInstance->GetNumMeshes(); index++)
		{
			const Model::MeshData& meshData = modelInstance->GetMeshData(index);

			if(modelInstance->GetMaterial())
			{
				modelInstance->GetMaterial()->SetAsResource(myMaterialBuffer);
			}

			DX11::Context->IASetInputLayout(meshData.myInputLayout.Get());
			DX11::Context->VSSetShader(meshData.myVertexShader.Get(), nullptr, 0);
			DX11::Context->PSSetShader(meshData.myPixelShader.Get(), nullptr, 0);
			DX11::Context->IASetIndexBuffer(meshData.myIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
			DX11::Context->IASetPrimitiveTopology(static_cast<D3D_PRIMITIVE_TOPOLOGY>(meshData.myPrimitiveTopology));

			DX11::Context->VSSetConstantBuffers(1, 1, myObjectBuffer.GetAddressOf());
			DX11::Context->PSSetConstantBuffers(1, 1, myObjectBuffer.GetAddressOf());
			DX11::Context->PSSetConstantBuffers(2, 1, myMaterialBuffer.GetAddressOf());


			SetSamplerState(SamplerState::SS_Wrap, 1);

			if(isInstanced && !model.myModel->HasBeenRendered())
			{

				myInstancedTransformBufferData.clear();
				int sizeOfDrawTransform = model.myModel->GetNumberOfInstances();
				for(int i = 0; i < sizeOfDrawTransform; i++)
				{
					auto matrix = model.myModel->GetTransformVector()[i].World->GetMatrix();
					myInstancedTransformBufferData.push_back(matrix);
				}


				ZeroMemory(&bufferData, sizeof(D3D11_MAPPED_SUBRESOURCE));
				result = DX11::Context->Map(myInstanceBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferData);
				memcpy(bufferData.pData, &myInstancedTransformBufferData[0], sizeof(Matrix4x4f) * model.myModel->GetNumberOfInstances());
				DX11::Context->Unmap(myInstanceBuffer.Get(), 0);

				ID3D11Buffer* buffers[2] = { meshData.myVertexBuffer.Get(), myInstanceBuffer.Get() };

				UINT stride[2] = { meshData.myStride, sizeof(Matrix4x4f) };

				UINT offset[2] = { meshData.myOffset, 0 };

				DX11::Context->IASetVertexBuffers(0, 2, buffers, stride, offset);
				DX11::Context->DrawIndexedInstanced(
					meshData.myNumberOfIndices,
					modelInstance->GetNumberOfInstances(),
					0, 0, 0
				);

			}
			else if(!isInstanced)
			{
				DX11::Context->IASetVertexBuffers(0, 1, meshData.myVertexBuffer.GetAddressOf(), &meshData.myStride, &meshData.myOffset);
				DX11::Context->IASetIndexBuffer(meshData.myIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
				DX11::Context->IASetPrimitiveTopology((D3D_PRIMITIVE_TOPOLOGY)meshData.myPrimitiveTopology);
				DX11::Context->DrawIndexed(meshData.myNumberOfIndices, 0, 0);
			}
		}

		if(isInstanced)
		{
			modelInstance->SetHasBeenRenderer(true);
		}

	}

}

void ForwardRenderer::RenderSprites(std::vector<RenderBuffer2D>& aSpriteList,
	const std::shared_ptr<DirectionalLight>& aDirectionalLight,
	const std::shared_ptr<EnvironmentLight>& anEnvironmentLight)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE bufferData;

	myFrameBufferData.View = Matrix4x4f::GetFastInverse(Renderer::GetViewMatrix());
	myFrameBufferData.Projection = Renderer::GetProjectionMatrix();

	ZeroMemory(&bufferData, sizeof(D3D11_MAPPED_SUBRESOURCE));
	result = DX11::Context->Map(myFrameBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferData);
	if(FAILED(result))
	{
		// it ded
	}
	memcpy(bufferData.pData, &myFrameBufferData, sizeof(FrameBufferData));

	DX11::Context->Unmap(myFrameBuffer.Get(), 0);
	SetDepthStencilState(DepthStencilState::DSS_ReadOnly);


	DX11::Context->VSSetConstantBuffers(0, 1, myFrameBuffer.GetAddressOf());
	DX11::Context->GSSetConstantBuffers(0, 1, myFrameBuffer.GetAddressOf());
	DX11::Context->PSSetConstantBuffers(0, 1, myFrameBuffer.GetAddressOf());
	DX11::Context->VSSetConstantBuffers(1, 1, myObjectBuffer.GetAddressOf());
	DX11::Context->PSSetConstantBuffers(1, 1, myObjectBuffer.GetAddressOf());
	CommonUtilities::MergeSort(aSpriteList);


	for(RenderBuffer2D rc : aSpriteList)
	{
		ZeroMemory(&bufferData, sizeof(D3D11_MAPPED_SUBRESOURCE));
		result = DX11::Context->Map(myObjectBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferData);
		if(FAILED(result))
		{
			// it ded
		}
		memcpy(bufferData.pData, &myObjectBufferData, sizeof(ObjectBufferData));
		DX11::Context->Unmap(myObjectBuffer.Get(), 0);

		SetDepthStencilState(DepthStencilState::DSS_ReadOnly);

		SetSamplerState(SamplerState::SS_Wrap, 1);

		rc.mySprite->SetAsResource();
		rc.mySprite->Draw();
	}
}

void ForwardRenderer::BuildDepth()
{}

void ForwardRenderer::ClearBuffer()
{}

void ForwardRenderer::Release()
{
	myFrameBuffer->Release();
	myObjectBuffer->Release();
	myLightBuffer->Release();
	myMaterialBuffer->Release();

	/*for (int i = 0; i < myDepthStencilStates.size(); i++)
	{
		myDepthStencilStates[i] = nullptr;
	}*/
}
