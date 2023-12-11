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
	result = DX11::Get().GetDevice()->CreateBuffer(&bufferDescription, nullptr, myFrameBuffer.GetAddressOf());
	if(FAILED(result))
	{
		return false;
	}

	bufferDescription.ByteWidth = sizeof(ObjectBufferData);
	result = DX11::Get().GetDevice()->CreateBuffer(&bufferDescription, nullptr, myObjectBuffer.GetAddressOf());
	if(FAILED(result))
	{
		return false;
	}

	bufferDescription.ByteWidth = sizeof(Vector4f);
	result = DX11::Get().GetDevice()->CreateBuffer(&bufferDescription, nullptr, myMaterialBuffer.GetAddressOf());
	if(FAILED(result))
	{
		return false;
	}

	bufferDescription.ByteWidth = sizeof(PointLightView);
	result = DX11::Get().GetDevice()->CreateBuffer(&bufferDescription, nullptr, myPointLightBuffer.GetAddressOf());
	if(FAILED(result))
	{
		return false;
	}

	std::ifstream gsFile;
	gsFile.open("Shaders\\ShadowCube_GS.cso", std::ios::binary);
	std::string gsData = { std::istreambuf_iterator(gsFile), std::istreambuf_iterator<char>() };
	DX11::Get().GetDevice()->CreateGeometryShader(gsData.data(), gsData.size(), nullptr, myShadowGeometryShader.GetAddressOf());
	gsFile.close();

	myDeferredVS = TextureAssetHandler::GetVertexShader("Shaders\\Fullscreen_VS.cso");
	myDeferredPS = TextureAssetHandler::GetPixelShader("Shaders\\Deferred_PS.cso");
	return true;
}

void ShadowRenderer::Render(Light* aLight, const std::vector<RenderBuffer>& aModelList)
{
	if(aModelList.empty())
	{
		return;
	}

	if(!aLight->HasShadowMap())
	{
		return;
	}

	

	HRESULT result = S_FALSE;
	D3D11_MAPPED_SUBRESOURCE bufferData;


	//aLight->SetAsResource(nullptr);

	const Light::LightBufferData lightData = aLight->GetLightBufferData();
	bool isCubeMap = lightData.LightType == 2;

	myFrameBufferData.View = lightData.LightView[0];
	myFrameBufferData.Projection = lightData.LightProjection;
	myFrameBufferData.CamTranslation = lightData.Position;
	myFrameBufferData.RenderMode = static_cast<unsigned int>(0);


	
	DX11::Get().GetContext()->RSSetViewports(1, &aLight->GetViewport());


	DX11::Get().GetContext()->PSSetShader(nullptr, nullptr, 0);
	DX11::Get().GetContext()->GSSetShader(nullptr, nullptr, 0);

	ZeroMemory(&bufferData, sizeof(D3D11_MAPPED_SUBRESOURCE));
	result = DX11::Get().GetContext()->Map(myFrameBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferData);
	memcpy(bufferData.pData, &myFrameBufferData, sizeof(FrameBufferData));
	DX11::Get().GetContext()->Unmap(myFrameBuffer.Get(), 0);
	DX11::Get().GetContext()->VSSetConstantBuffers(0, 1, myFrameBuffer.GetAddressOf());
	DX11::Get().GetContext()->PSSetConstantBuffers(0, 1, myFrameBuffer.GetAddressOf());

	if(isCubeMap)
	{
		DX11::Get().GetContext()->GSSetConstantBuffers(0, 1, myFrameBuffer.GetAddressOf());
		myPointLightView.myPointLightViews[0] = lightData.LightView[0];
		myPointLightView.myPointLightViews[1] = lightData.LightView[1];
		myPointLightView.myPointLightViews[2] = lightData.LightView[2];
		myPointLightView.myPointLightViews[3] = lightData.LightView[3];
		myPointLightView.myPointLightViews[4] = lightData.LightView[4];
		myPointLightView.myPointLightViews[5] = lightData.LightView[5];

		myPointLightView.SC_LightTranslation = lightData.Position;
		myPointLightView.SC_Projection = lightData.LightProjection;


		ZeroMemory(&bufferData, sizeof(D3D11_MAPPED_SUBRESOURCE));
		DX11::Get().GetContext()->Map(myPointLightBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferData);
		memcpy(bufferData.pData, &myPointLightView, sizeof(PointLightView));
		DX11::Get().GetContext()->Unmap(myPointLightBuffer.Get(), 0);
		DX11::Get().GetContext()->GSSetConstantBuffers(5, 1, myPointLightBuffer.GetAddressOf());
	}

	ModelAssetHandler::Get().ResetRenderedModels();

	aLight->ClearShadowMap();
	aLight->SetShadowMapAsDepth();

	for(const auto& RenderBuffer : aModelList)
	{
		if(!RenderBuffer.myModel)
		{
			continue;
		}
		auto& model = RenderBuffer.myModel;
		bool isInstanced = false; //model->HasRenderedInstance();

		for(unsigned int m = 0; m < model->GetNumMeshes(); m++)
		{
			const ModelInstance::MeshData& meshData = model->GetMeshData(m);

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

			result = DX11::Get().GetContext()->Map(myObjectBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferData);
			if(FAILED(result))
			{

			}

			memcpy(bufferData.pData, &myObjectBufferData, sizeof(ObjectBufferData));
			DX11::Get().GetContext()->Unmap(myObjectBuffer.Get(), 0);
			
			DX11::Get().GetContext()->VSSetConstantBuffers(1, 1, myObjectBuffer.GetAddressOf());
			DX11::Get().GetContext()->PSSetConstantBuffers(1, 1, myObjectBuffer.GetAddressOf());

			//if(!model->GetMaterial().empty())
			//{
				//model->GetMaterial()[0]->SetAsResource(myMaterialBuffer.Get());

				//if(model->RenderWithDeferred())
				//{
				//	DX11::Get().GetContext()->VSSetShader(meshData.myVertexShader.Get(), nullptr, 0);
				//	DX11::Get().GetContext()->PSSetShader(myDeferredPS.Get(), nullptr, 0);
				//}
				//else
				//{
					DX11::Get().GetContext()->VSSetShader(meshData.myVertexShader.Get(), nullptr, 0);
					//DX11::Get().GetContext()->PSSetShader(meshData.myPixelShader.Get(), nullptr, 0);
				//}
				//DX11::Get().GetContext()->PSSetConstantBuffers(2, 1, myMaterialBuffer.GetAddressOf());
			//}

			DX11::Get().GetContext()->IASetInputLayout(meshData.myInputLayout.Get());

			if(isCubeMap)
			{
				DX11::Get().GetContext()->GSSetConstantBuffers(0, 1, myFrameBuffer.GetAddressOf());
				DX11::Get().GetContext()->GSSetConstantBuffers(5, 1, myPointLightBuffer.GetAddressOf());
				DX11::Get().GetContext()->GSSetShader(myShadowGeometryShader.Get(), nullptr, 0);
			}
			else
			{
				DX11::Get().GetContext()->GSSetShader(nullptr, nullptr, 0);
			}


		
			

			DX11::Get().GetContext()->IASetIndexBuffer(meshData.myIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
			DX11::Get().GetContext()->IASetPrimitiveTopology((D3D_PRIMITIVE_TOPOLOGY)meshData.myPrimitiveTopology);

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
				result = DX11::Get().GetContext()->Map(myInstanceBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferData);
				memcpy(bufferData.pData, &myInstancedTransformBufferData[0], sizeof(Matrix4x4f) * model->GetNumberOfInstances());
				DX11::Get().GetContext()->Unmap(myInstanceBuffer.Get(), 0);

				ID3D11Buffer* buffers[2] = { meshData.myVertexBuffer.Get(), myInstanceBuffer.Get() };

				UINT stride[2] = { meshData.myStride, sizeof(Matrix4x4f) };

				UINT offset[2] = { meshData.myOffset, 0 };

				DX11::Get().GetContext()->IASetVertexBuffers(0, 2, buffers, stride, offset);
				DX11::Get().GetContext()->DrawIndexedInstanced(
					meshData.myNumberOfIndices,
					model->GetNumberOfInstances(),
					0, 0, 0
				);

			}
			else
			{*/
				DX11::Get().GetContext()->IASetVertexBuffers(0, 1, meshData.myVertexBuffer.GetAddressOf(), &meshData.myStride, &meshData.myOffset);
				DX11::Get().GetContext()->DrawIndexed(meshData.myNumberOfIndices, 0, 0);
			//}
		}

		/*if(isInstanced)
		{
			model->SetHasBeenRenderer(true);
		}*/
	}


	DX11::Get().GetContext()->GSSetShader(nullptr, nullptr, 0);
	DX11::Get().ResetViewport();
	//DX11::Get().GetContext()->OMSetRenderTargets(0, nullptr, nullptr);
}

void ShadowRenderer::ClearResources()
{
	ID3D11ShaderResourceView* srvnull = nullptr;
	//DX11::Get().GetContext()->VSSetShaderResources(19, 1, &srvnull);
	DX11::Get().GetContext()->PSSetShaderResources(19, 1, &srvnull);

	for(int i = 0; i < 40; i++)
	{
		//DX11::Get().GetContext()->VSSetShaderResources(20 + i, 1, &srvnull);
		DX11::Get().GetContext()->PSSetShaderResources(20 + i, 1, &srvnull);
	}
}

void ShadowRenderer::ClearTarget()
{

}
