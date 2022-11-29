#include "GraphicsEngine.pch.h"
#include "Render/ShadowRenderer.h"

#include <fstream>

#include "GraphicsEngine.h"
#include "Renderer.h"
#include "Framework/DX11.h"
#include <Sort/sort.hpp>

#include "AssetHandlers/ModelAssetHandler.h"

bool ShadowRenderer::Initialize()
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

	bufferDescription.ByteWidth = sizeof(PointLightView);
	result = DX11::Device->CreateBuffer(&bufferDescription, nullptr, myPointLightBuffer.GetAddressOf());
	if(FAILED(result))
	{
		return false;
	}

	std::ifstream gsFile;
	gsFile.open("Shaders\\ShadowCubeGS.cso", std::ios::binary);
	std::string gsData = { std::istreambuf_iterator(gsFile), std::istreambuf_iterator<char>() };
	DX11::Device->CreateGeometryShader(gsData.data(), gsData.size(), nullptr, myShadowGeometryShader.GetAddressOf());
	gsFile.close();
	return true;
}

void ShadowRenderer::Render(const std::vector<RenderBuffer>& aModelList)
{
	if(aModelList.empty())
	{
		return;
	}


	HRESULT result = S_FALSE;
	D3D11_MAPPED_SUBRESOURCE bufferData;

	std::vector<Light*> lightList = Renderer::GetLights();

	for(int i = 0; i < lightList.size(); i++)
	{
		if(!lightList[i]->HasShadowMap())
		{
			continue;
		}

		lightList[i]->ClearShadowMap();
		lightList[i]->SetShadowMapAsDepth();



		const Light::LightBufferData lightData = lightList[i]->GetLightBufferData();
		bool isCubeMap = lightData.LightType == 2;

		myFrameBufferData.View = lightData.LightView;
		myFrameBufferData.CamTranslation = lightData.Position;
		myFrameBufferData.Projection = lightData.LightProjection;
		
		myFrameBufferData.RenderMode = static_cast<unsigned int>(0);

		DX11::Context->RSSetViewports(1, &lightList[i]->GetViewport());
		

		DX11::Context->PSSetShader(nullptr, nullptr, 0);
		DX11::Context->GSSetShader(nullptr, nullptr, 0);

		ZeroMemory(&bufferData, sizeof(D3D11_MAPPED_SUBRESOURCE));
		result = DX11::Context->Map(myFrameBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferData);
		memcpy(bufferData.pData, &myFrameBufferData, sizeof(FrameBufferData));
		DX11::Context->Unmap(myFrameBuffer.Get(), 0);
		DX11::Context->VSSetConstantBuffers(0, 1, myFrameBuffer.GetAddressOf());
		DX11::Context->PSSetConstantBuffers(0, 1, myFrameBuffer.GetAddressOf());

		if(isCubeMap)
		{
			DX11::Context->GSSetConstantBuffers(0, 1, myFrameBuffer.GetAddressOf());
			myPointLightView.myPointLightViews[0] = lightData.LightView;
			myPointLightView.myPointLightViews[1] = lightData.LightView1;
			myPointLightView.myPointLightViews[2] = lightData.LightView2;
			myPointLightView.myPointLightViews[3] = lightData.LightView3;
			myPointLightView.myPointLightViews[4] = lightData.LightView4;
			myPointLightView.myPointLightViews[5] = lightData.LightView5;
			ZeroMemory(&bufferData, sizeof(D3D11_MAPPED_SUBRESOURCE));
			DX11::Context->Map(myPointLightBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferData);
			memcpy(bufferData.pData, &myPointLightView, sizeof(PointLightView));
			DX11::Context->Unmap(myPointLightBuffer.Get(), 0);
			DX11::Context->GSSetConstantBuffers(5, 1, myPointLightBuffer.GetAddressOf());
		}

		ModelAssetHandler::ResetRenderedModels();

		for(const auto& RenderBuffer : aModelList)
		{
			if(!RenderBuffer.myModel)
			{
				continue;
			}
			auto& model = RenderBuffer.myModel;
			bool isInstanced = model->HasRenderedInstance();

			for(unsigned int m = 0; m < model->GetNumMeshes(); m++)
			{
				const Model::MeshData& meshData = model->GetMeshData(m);


				

				myObjectBufferData.IsInstanced = isInstanced;
				myObjectBufferData.World = RenderBuffer.myTransform;
				ZeroMemory(&bufferData, sizeof(D3D11_MAPPED_SUBRESOURCE));

				
				myObjectBufferData.myHasBones = false;
				if(model->GetSkeleton()->GetRoot())
				{
					auto bones = model->GetBoneTransform();
					myObjectBufferData.myHasBones = true;
					memcpy_s(
						&myObjectBufferData.BoneData[0], sizeof(Matrix4x4f) * MAX_MODEL_BONES,
						&bones[0], sizeof(Matrix4x4f) * MAX_MODEL_BONES
					);
				}

				result = DX11::Context->Map(myObjectBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferData);
				if(FAILED(result))
				{

				}

				memcpy(bufferData.pData, &myObjectBufferData, sizeof(ObjectBufferData));
				DX11::Context->Unmap(myObjectBuffer.Get(), 0);
				DX11::Context->VSSetConstantBuffers(1, 1, myObjectBuffer.GetAddressOf());


				if(model->GetMaterial())
				{
					DX11::Context->VSSetShader(meshData.myVertexShader.Get(), nullptr, 0);
					DX11::Context->GSSetShader(meshData.myGeometryShader.Get(), nullptr, 0);
					DX11::Context->PSSetShader(meshData.myPixelShader.Get(), nullptr, 0);
					model->GetMaterial()->SetAsResource(myMaterialBuffer);
				}

				DX11::Context->IASetInputLayout(meshData.myInputLayout.Get());

				if(isCubeMap)
				{
					DX11::Context->GSSetConstantBuffers(0, 1, myFrameBuffer.GetAddressOf());
					DX11::Context->GSSetConstantBuffers(5, 1, myPointLightBuffer.GetAddressOf());
					DX11::Context->GSSetShader(myShadowGeometryShader.Get(), nullptr, 0);
				}
				else
				{
					DX11::Context->GSSetShader(nullptr, nullptr, 0);
				}
				DX11::Context->PSSetShader(nullptr, nullptr, 0);

				DX11::Context->VSSetConstantBuffers(1, 1, myObjectBuffer.GetAddressOf());

				
				DX11::Context->IASetIndexBuffer(meshData.myIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
				DX11::Context->IASetPrimitiveTopology((D3D_PRIMITIVE_TOPOLOGY)meshData.myPrimitiveTopology);

				if(isInstanced && !model->HasBeenRendered())
				{
					myInstancedTransformBufferData.clear();
					int sizeOfDrawTransform = model->GetNumberOfInstances();
					for(int i = 0; i < sizeOfDrawTransform; i++)
					{
						auto matrix = model->GetTransformVector()[i].World->GetMatrix();
						myInstancedTransformBufferData.push_back(matrix);
					}


					ZeroMemory(&bufferData, sizeof(D3D11_MAPPED_SUBRESOURCE));
					result = DX11::Context->Map(myInstanceBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferData);
					memcpy(bufferData.pData, &myInstancedTransformBufferData[0], sizeof(Matrix4x4f) * model->GetNumberOfInstances());
					DX11::Context->Unmap(myInstanceBuffer.Get(), 0);

					ID3D11Buffer* buffers[2] = { meshData.myVertexBuffer.Get(), myInstanceBuffer.Get() };

					UINT stride[2] = { meshData.myStride, sizeof(Matrix4x4f) };

					UINT offset[2] = { meshData.myOffset, 0 };

					DX11::Context->IASetVertexBuffers(0, 2, buffers, stride, offset);
					DX11::Context->DrawIndexedInstanced(
						meshData.myNumberOfIndices,
						model->GetNumberOfInstances(),
						0, 0, 0
					);

				}
				else if (!isInstanced)
				{
					DX11::Context->IASetVertexBuffers(0, 1, meshData.myVertexBuffer.GetAddressOf(), &meshData.myStride, &meshData.myOffset);
					DX11::Context->DrawIndexed(meshData.myNumberOfIndices, 0, 0);
				}
			}

			if(isInstanced)
			{
				model->SetHasBeenRenderer(true);
			}
		}
	}
	
	DX11::Context->GSSetShader(nullptr, nullptr, 0);
	DX11::ResizeViewport();
}

void ShadowRenderer::ClearResources()
{
	ID3D11ShaderResourceView* srvnull = nullptr;
	DX11::Context->PSSetShaderResources(19, 1, &srvnull);

	for (int i = 0; i < 40; i++)
	{
		DX11::Context->PSSetShaderResources(20 + i, 1, &srvnull);
	}
}
