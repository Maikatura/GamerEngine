#include "GraphicsEngine.pch.h"
#include "ForwardRenderer.h"
#include <Renderer/Framework/DX11.h>
#include "Renderer/GraphicsEngine.h"
#include "Renderer.h"
#include "Renderer/Light/DirectionalLight.h"
#include "Renderer/Light/EnvironmentLight.h"
#include "Sort/Sort.hpp"
#include <Renderer/Particles/particleemitter.h>

#include "Renderer/AssetHandlers/ModelAssetHandler.h"

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

	/*bufferDescription.ByteWidth = sizeof(Vector4f);
	result = DX11::Device->CreateBuffer(&bufferDescription, nullptr, myMaterialBuffer.GetAddressOf());
	if(FAILED(result))
	{
		return false;
	}*/

	bufferDescription.ByteWidth = sizeof(SceneLightBuffer);
	result = DX11::Device->CreateBuffer(&bufferDescription, nullptr, myLightBuffer.GetAddressOf());
	if(FAILED(result))
	{
		return false;
	}


	BuildDepth();

	return true;
}

void ForwardRenderer::Render(Matrix4x4f aView, Matrix4x4f aProjection, const std::vector<RenderBuffer>& aModelList, const std::shared_ptr<DirectionalLight>& aDirectionalLight, const std::shared_ptr<EnvironmentLight>& anEnvironmentLight, const std::vector<Light*>& aLightList, VREye anEye)
{
	if(!Renderer::GetCamera())
	{
		return;
	}


	if(aModelList.empty())
	{
		return;
	}


	HRESULT result = S_FALSE;
	D3D11_MAPPED_SUBRESOURCE bufferData;





	myFrameBufferData.View = Matrix4x4f::GetFastInverse(aView);
	myFrameBufferData.CamTranslation = aView.GetPosition();
	myFrameBufferData.Projection = aProjection;
	myFrameBufferData.RenderMode = static_cast<int>(GraphicsEngine::Get()->GetRenderMode());
	myFrameBufferData.FarPlane = Renderer::GetCamera()->myFarPlane;
	myFrameBufferData.NearPlane = Renderer::GetCamera()->myNearPlane;


	RECT clientRect = DX11::GetClientSize();
	uint32_t width = anEye == VREye::None ? clientRect.right - clientRect.left : DX11::m_nRenderWidth;
	uint32_t height = anEye == VREye::None ? clientRect.bottom - clientRect.top : DX11::m_nRenderHeight;

	const Vector2ui Resolution = {
		width,
		height
	};
	myFrameBufferData.Resolution = Resolution;

	ZeroMemory(&bufferData, sizeof(D3D11_MAPPED_SUBRESOURCE));
	result = DX11::GetContext()->Map(myFrameBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferData);
	if (FAILED(result))
	{
		// BOOM?
		return;
	}

	memcpy(bufferData.pData, &myFrameBufferData, sizeof(FrameBufferData));
	DX11::GetContext()->Unmap(myFrameBuffer.Get(), 0);

	DX11::GetContext()->VSSetConstantBuffers(0, 1, myFrameBuffer.GetAddressOf());
	DX11::GetContext()->PSSetConstantBuffers(0, 1, myFrameBuffer.GetAddressOf());

	for(const RenderBuffer& modelBuffer : aModelList)
	{
		std::shared_ptr<ModelInstance> model = modelBuffer.myModel;


		if(model == nullptr)
		{
			return;
		}

		if(model->RenderWithDeferred())
		{
			continue;
		}

		

		/*SetBlendState(BlendState::Alpha);
		SetDepthStencilState(DepthStencilState::DSS_ReadWrite);*/

		ModelAssetHandler::Get().ResetRenderedModels();

		if(aDirectionalLight)
		{
			mySceneLightBufferData.DirectionalLight = aDirectionalLight->GetLightBufferData();
			aDirectionalLight->SetAsResource(nullptr);
		}

		if(anEnvironmentLight)
		{
			anEnvironmentLight->SetAsResource(nullptr);
		}

		mySceneLightBufferData.NumLights = 0;
		ZeroMemory(mySceneLightBufferData.Lights, sizeof(Light::LightBufferData) * MAX_FORWARD_LIGHTS);
		for(size_t l = 0; l < aLightList.size() && l < MAX_FORWARD_LIGHTS; l++)
		{
			if(aLightList[l]->GetLightBufferData().LightType == 1)
			{
				continue;
			}
			else
			{
				mySceneLightBufferData.Lights[l] = aLightList[l]->GetLightBufferData();
				mySceneLightBufferData.NumLights++;
				aLightList[l]->SetAsResource(nullptr);
			}
		}


		result = DX11::GetContext()->Map(
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

		DX11::GetContext()->Unmap(myLightBuffer.Get(), 0);
		DX11::GetContext()->PSSetConstantBuffers(3, 1, myLightBuffer.GetAddressOf());

		ModelAssetHandler::Get().ResetRenderedModels();



		std::shared_ptr<ModelInstance> modelInstance = model;

		//bool isInstanced = modelInstance->HasRenderedInstance();
		bool isInstanced = false;

		myObjectBufferData.IsInstanced = isInstanced;
		myObjectBufferData.World = modelBuffer.myTransform;
		myObjectBufferData.WorldInv = Matrix4x4f::GetFastInverse(modelBuffer.myTransform);
		ZeroMemory(&bufferData, sizeof(D3D11_MAPPED_SUBRESOURCE));

		myObjectBufferData.myHasBones = false;
		if(model->GetSkeleton()->GetRoot())
		{
			auto bones = modelInstance->GetBoneTransform();
			myObjectBufferData.myHasBones = true;
			memcpy_s(
				&myObjectBufferData.BoneData[0], sizeof(Matrix4x4f) * MAX_MODEL_BONES,
				&bones[0], sizeof(Matrix4x4f) * MAX_MODEL_BONES
			);
		}

		result = DX11::GetContext()->Map(myObjectBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferData);
		if(FAILED(result))
		{
			// WTF
		}

		memcpy(bufferData.pData, &myObjectBufferData, sizeof(ObjectBufferData));
		DX11::GetContext()->Unmap(myObjectBuffer.Get(), 0);

		DX11::GetContext()->VSSetConstantBuffers(1, 1, myObjectBuffer.GetAddressOf());
		DX11::GetContext()->PSSetConstantBuffers(1, 1, myObjectBuffer.GetAddressOf());

		for(int index = 0; index < modelInstance->GetNumMeshes(); index++)
		{
			const Model::MeshData& meshData = modelInstance->GetMeshData(index);

			DX11::GetContext()->IASetInputLayout(meshData.myInputLayout.Get());
			DX11::GetContext()->VSSetShader(meshData.myVertexShader.Get(), nullptr, 0);
			DX11::GetContext()->PSSetShader(meshData.myPixelShader.Get(), nullptr, 0);
			DX11::GetContext()->IASetIndexBuffer(meshData.myIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
			DX11::GetContext()->IASetPrimitiveTopology(static_cast<D3D_PRIMITIVE_TOPOLOGY>(meshData.myPrimitiveTopology)); // Use D3D11_PRIMITIVE_TOPOLOGY_... for your specific primitive type

			if(!model->GetMaterial().empty() && static_cast<int>(meshData.myMaterialIndex) < model->GetMaterialSize())
			{
				model->GetMaterial()[meshData.myMaterialIndex]->SetAsResource(nullptr);
			}
			else if(!model->GetMaterial().empty())
			{
				model->GetMaterial()[0]->SetAsResource(nullptr);
			}

			/*DX11::GetContext()->VSSetConstantBuffers(2, 1, myMaterialBuffer.GetAddressOf());
			DX11::GetContext()->PSSetConstantBuffers(2, 1, myMaterialBuffer.GetAddressOf());
*/

//SetSamplerState(SamplerState::SS_Wrap);

			/*if(isInstanced && !model->HasBeenRendered())
			{
				myInstancedTransformBufferData.clear();
				std::vector<ModelInstance::RenderedInstanceData> myTransformData = model->GetTransformVector();
				for(int i = 0; i < myTransformData.size(); i++)
				{
					auto matrix = myTransformData[i].World->GetMatrix();
					myInstancedTransformBufferData.push_back(matrix);
				}

				ZeroMemory(&bufferData, sizeof(D3D11_MAPPED_SUBRESOURCE));
				result = DX11::GetContext()->Map(myInstanceBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferData);
				memcpy(bufferData.pData, &myInstancedTransformBufferData[0], sizeof(Matrix4x4f) * model->GetNumberOfInstances());
				DX11::GetContext()->Unmap(myInstanceBuffer.Get(), 0);

				ID3D11Buffer* buffers[2] = { meshData.myVertexBuffer.Get(), myInstanceBuffer.Get() };

				UINT stride[2] = { meshData.myStride, sizeof(Matrix4x4f) };

				UINT offset[2] = { meshData.myOffset, 0 };

				DX11::GetContext()->IASetVertexBuffers(0, 2, buffers, stride, offset);
				DX11::GetContext()->DrawIndexedInstanced(
					meshData.myNumberOfIndices,
					model->GetNumberOfInstances(),
					0, 0, 0
				);
			}
			else
			{*/
				DX11::GetContext()->IASetVertexBuffers(0, 1, meshData.myVertexBuffer.GetAddressOf(), &meshData.myStride, &meshData.myOffset);
				DX11::GetContext()->DrawIndexed(meshData.myNumberOfIndices, 0, 0);
			//}
		}

		//if(isInstanced)
		//{
		//	modelInstance->SetHasBeenRenderer(true);
		//}

	}

}

void ForwardRenderer::RenderSprites(Matrix4x4f aView, Matrix4x4f aProjection, std::vector<RenderBuffer2D>& aSpriteList,
	const std::shared_ptr<DirectionalLight>& aDirectionalLight,
	const std::shared_ptr<EnvironmentLight>& anEnvironmentLight)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE bufferData;

	myFrameBufferData.View = Matrix4x4f::GetFastInverse(aView);
	myFrameBufferData.Projection = aProjection;

	ZeroMemory(&bufferData, sizeof(D3D11_MAPPED_SUBRESOURCE));
	result = DX11::GetContext()->Map(myFrameBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferData);
	if(FAILED(result))
	{
		// it ded
	}
	memcpy(bufferData.pData, &myFrameBufferData, sizeof(FrameBufferData));

	DX11::GetContext()->Unmap(myFrameBuffer.Get(), 0);
	//SetDepthStencilState(DepthStencilState::DSS_ReadOnly);


	DX11::GetContext()->VSSetConstantBuffers(0, 1, myFrameBuffer.GetAddressOf());
	DX11::GetContext()->GSSetConstantBuffers(0, 1, myFrameBuffer.GetAddressOf());
	DX11::GetContext()->PSSetConstantBuffers(0, 1, myFrameBuffer.GetAddressOf());
	DX11::GetContext()->VSSetConstantBuffers(1, 1, myObjectBuffer.GetAddressOf());
	DX11::GetContext()->PSSetConstantBuffers(1, 1, myObjectBuffer.GetAddressOf());
	CommonUtilities::MergeSort(aSpriteList);


	for(RenderBuffer2D rc : aSpriteList)
	{
		ZeroMemory(&bufferData, sizeof(D3D11_MAPPED_SUBRESOURCE));
		result = DX11::GetContext()->Map(myObjectBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferData);
		if(FAILED(result))
		{
			// it ded
		}
		memcpy(bufferData.pData, &myObjectBufferData, sizeof(ObjectBufferData));
		DX11::GetContext()->Unmap(myObjectBuffer.Get(), 0);

		//SetDepthStencilState(DepthStencilState::DSS_ReadOnly);

		//SetSamplerState(SamplerState::SS_Wrap);

		if(rc.mySprite)
		{
			rc.mySprite->SetAsResource();
			rc.mySprite->Draw();
		}

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
	//myMaterialBuffer->Release();

	/*for (int i = 0; i < myDepthStencilStates.size(); i++)
	{
		myDepthStencilStates[i] = nullptr;
	}*/
}
