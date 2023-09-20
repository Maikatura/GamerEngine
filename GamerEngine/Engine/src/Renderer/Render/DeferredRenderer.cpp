#include <GraphicsEngine.pch.h>
#include <Renderer/Render/DeferredRenderer.h>

#include "ForwardRenderer.h"
#include "Renderer/GraphicsEngine.h"
#include "Renderer.h"
#include "Renderer/AssetHandlers/ModelAssetHandler.h"
#include "Renderer/Framework/DX11.h"

void GBuffer::SetAsTarget() const
{
	ID3D11RenderTargetView* rtvList[GBufferTexture::GBufferTexture_Count];
	for(int i = 0; i < myRTVs.size(); i++)
	{
		rtvList[i] = myRTVs[i].Get();
	}
	//rtvList[ID] = DX11::ourIDBuffer.Get();
	


	//DX11::GetContext()->OMSetRenderTargets(GBufferTexture::GBufferTexture_Count, &rtvList[0], DX11::DepthBuffer.Get());
}

void GBuffer::ClearTarget() const
{
	ID3D11RenderTargetView* rtvList[GBufferTexture::GBufferTexture_Count];
	for(int i = 0; i < GBufferTexture::GBufferTexture_Count; i++)
	{
		rtvList[i] = nullptr;
		/*Vector4f color = { 0,0,0,0 };
		DX11::GetContext()->ClearRenderTargetView(myRTVs[i].Get(), &color.x);*/
	}
	//DX11::GetContext()->OMSetRenderTargets(1, DX11::RenderRTV.GetAddressOf(), DX11::DepthBuffer.Get());
}

void GBuffer::SetAsResource(unsigned aStartSlot) const
{
	ID3D11ShaderResourceView* mySRVList[GBufferTexture::GBufferTexture_Count];
	for(int t = 0; t < GBufferTexture::GBufferTexture_Count; t++)
	{
		mySRVList[t] = mySRVs[t].Get();
	}

	DX11::GetContext()->VSSetShaderResources(aStartSlot, GBufferTexture::GBufferTexture_Count, &mySRVList[0]);
	DX11::GetContext()->PSSetShaderResources(aStartSlot, GBufferTexture::GBufferTexture_Count, &mySRVList[0]);
}

void GBuffer::ClearResource(unsigned aStartSlot) const
{

	ID3D11ShaderResourceView* srvList[GBufferTexture::GBufferTexture_Count];
	for(int i = 0; i < GBufferTexture::GBufferTexture_Count; i++)
	{
		srvList[i] = nullptr;
	}
	DX11::GetContext()->VSSetShaderResources(aStartSlot, GBufferTexture::GBufferTexture_Count, &srvList[0]);
	DX11::GetContext()->PSSetShaderResources(aStartSlot, GBufferTexture::GBufferTexture_Count, &srvList[0]);
}

void GBuffer::Clear()
{
	Vector4f clCol = { 0, 0, 0, 0 };
	for(int i = 0; i < GBufferTexture::GBufferTexture_Count; i++)
	{
		DX11::GetContext()->ClearRenderTargetView(myRTVs[i].Get(), &clCol.x);
	}
}

void GBuffer::Release()
{
	for(unsigned t = 0; t < mySRVs.size(); t++)
	{
		mySRVs[t].Reset();
	}

	for(unsigned t = 0; t < myRTVs.size(); t++)
	{
		myRTVs[t].Reset();
	}

	SafeRelease(myTexture);
}

bool GBuffer::CreateGBuffer()
{

	HRESULT result;

	RECT clientRect = DX11::GetClientSize();


	DXGI_SAMPLE_DESC sampleDesc{};
	sampleDesc.Quality = 0;
	sampleDesc.Count = 1;


	D3D11_TEXTURE2D_DESC bufferDesc = { 0 };
	bufferDesc.Width = static_cast<UINT>(clientRect.right - clientRect.left);
	bufferDesc.Height = static_cast<UINT>(clientRect.bottom - clientRect.top);
	bufferDesc.ArraySize = 1;
	bufferDesc.MipLevels = 1;
	bufferDesc.SampleDesc = sampleDesc;
	bufferDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	bufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	bufferDesc.CPUAccessFlags = 0;

	D3D11_RENDER_TARGET_VIEW_DESC renderDesc;
	renderDesc.Format = bufferDesc.Format;
	renderDesc.Texture2D.MipSlice = 0;
	renderDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

	D3D11_SHADER_RESOURCE_VIEW_DESC resourceViewDesc{};
	resourceViewDesc.Format = bufferDesc.Format;
	resourceViewDesc.Texture2D.MipLevels = bufferDesc.MipLevels;
	resourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	

	result = DX11::Device->CreateTexture2D(&bufferDesc, nullptr, &myTexture);
	if(FAILED(result))
		return false;
	result = DX11::Device->CreateRenderTargetView(myTexture, &renderDesc, myRTVs[GBuffer::GBufferTexture::GBufferTexture_Albedo].ReleaseAndGetAddressOf());
	if(FAILED(result))
		return false;
	result = DX11::Device->CreateShaderResourceView(myTexture, &resourceViewDesc, mySRVs[GBuffer::GBufferTexture::GBufferTexture_Albedo].ReleaseAndGetAddressOf());
	if(FAILED(result))
		return false;



	bufferDesc.Format = DXGI_FORMAT_R16G16B16A16_SNORM;
	renderDesc.Format = bufferDesc.Format;
	resourceViewDesc.Format = bufferDesc.Format;
	result = DX11::Device->CreateTexture2D(&bufferDesc, nullptr, &myTexture);
	if(FAILED(result))
		return false;
	result = DX11::Device->CreateRenderTargetView(myTexture, &renderDesc, myRTVs[GBuffer::GBufferTexture::GBufferTexture_Normal].ReleaseAndGetAddressOf());
	if(FAILED(result))
		return false;
	result = DX11::Device->CreateShaderResourceView(myTexture, &resourceViewDesc, mySRVs[GBuffer::GBufferTexture::GBufferTexture_Normal].ReleaseAndGetAddressOf());
	if(FAILED(result))
		return false;


	bufferDesc.Format = DXGI_FORMAT_R16G16B16A16_SNORM;
	renderDesc.Format = bufferDesc.Format;
	resourceViewDesc.Format = bufferDesc.Format;
	result = DX11::Device->CreateTexture2D(&bufferDesc, nullptr, &myTexture);
	if(FAILED(result))
		return false;
	result = DX11::Device->CreateRenderTargetView(myTexture, &renderDesc, myRTVs[GBuffer::GBufferTexture::GBufferTexture_Material].ReleaseAndGetAddressOf());
	if(FAILED(result))
		return false;
	result = DX11::Device->CreateShaderResourceView(myTexture, &resourceViewDesc, mySRVs[GBuffer::GBufferTexture::GBufferTexture_Material].ReleaseAndGetAddressOf());
	if(FAILED(result))
		return false;



	bufferDesc.Format = DXGI_FORMAT_R16G16B16A16_SNORM;
	renderDesc.Format = bufferDesc.Format;
	resourceViewDesc.Format = bufferDesc.Format;
	result = DX11::Device->CreateTexture2D(&bufferDesc, nullptr, &myTexture);
	if(FAILED(result))
		return false;
	result = DX11::Device->CreateRenderTargetView(myTexture, &renderDesc, myRTVs[GBuffer::GBufferTexture::GBufferTexture_VertexNormal].ReleaseAndGetAddressOf());
	if(FAILED(result))
		return false;
	result = DX11::Device->CreateShaderResourceView(myTexture, &resourceViewDesc, mySRVs[GBuffer::GBufferTexture::GBufferTexture_VertexNormal].ReleaseAndGetAddressOf());
	if(FAILED(result))
		return false;



	bufferDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	renderDesc.Format = bufferDesc.Format;
	resourceViewDesc.Format = bufferDesc.Format;
	result = DX11::Device->CreateTexture2D(&bufferDesc, nullptr, &myTexture);
	if(FAILED(result))
		return false;
	result = DX11::Device->CreateRenderTargetView(myTexture, &renderDesc, myRTVs[GBuffer::GBufferTexture::GBufferTexture_Position].ReleaseAndGetAddressOf());
	if(FAILED(result))
		return false;
	result = DX11::Device->CreateShaderResourceView(myTexture, &resourceViewDesc, mySRVs[GBuffer::GBufferTexture::GBufferTexture_Position].ReleaseAndGetAddressOf());
	if(FAILED(result))
		return false;


	bufferDesc.Format = DXGI_FORMAT_R8_UNORM;
	renderDesc.Format = bufferDesc.Format;
	resourceViewDesc.Format = bufferDesc.Format;
	result = DX11::Device->CreateTexture2D(&bufferDesc, nullptr, &myTexture);
	if(FAILED(result))
		return false;
	result = DX11::Device->CreateRenderTargetView(myTexture, &renderDesc, myRTVs[GBuffer::GBufferTexture::GBufferTexture_AmbientOcclusion].ReleaseAndGetAddressOf());
	if(FAILED(result))
		return false;
	result = DX11::Device->CreateShaderResourceView(myTexture, &resourceViewDesc, mySRVs[GBuffer::GBufferTexture::GBufferTexture_AmbientOcclusion].ReleaseAndGetAddressOf());
	if(FAILED(result))
		return false;



	bufferDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	renderDesc.Format = bufferDesc.Format;
	resourceViewDesc.Format = bufferDesc.Format;
	result = DX11::Device->CreateTexture2D(&bufferDesc, nullptr, &myTexture);
	if(FAILED(result))
		return false;
	result = DX11::Device->CreateRenderTargetView(myTexture, &renderDesc, myRTVs[GBuffer::GBufferTexture::GBufferTexture_ViewPosition].ReleaseAndGetAddressOf());
	if(FAILED(result))
		return false;
	result = DX11::Device->CreateShaderResourceView(myTexture, &resourceViewDesc, mySRVs[GBuffer::GBufferTexture::GBufferTexture_ViewPosition].ReleaseAndGetAddressOf());
	if(FAILED(result))
		return false;

	bufferDesc.Format = DXGI_FORMAT_R16G16B16A16_SNORM;
	renderDesc.Format = bufferDesc.Format;
	resourceViewDesc.Format = bufferDesc.Format;
	result = DX11::Device->CreateTexture2D(&bufferDesc, nullptr, &myTexture);
	if(FAILED(result))
		return false;
	result = DX11::Device->CreateRenderTargetView(myTexture, &renderDesc, myRTVs[GBuffer::GBufferTexture::GBufferTexture_ViewNormal].ReleaseAndGetAddressOf());
	if(FAILED(result))
		return false;
	result = DX11::Device->CreateShaderResourceView(myTexture, &resourceViewDesc, mySRVs[GBuffer::GBufferTexture::GBufferTexture_ViewNormal].ReleaseAndGetAddressOf());
	if(FAILED(result))
		return false;


	return true;
}

bool DeferredRenderer::Initialize()
{
	HRESULT result;

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


	/*D3D11_BUFFER_DESC constantBufferDesc;
	ZeroMemory(&constantBufferDesc, sizeof(constantBufferDesc));
	constantBufferDesc.ByteWidth = sizeof(BlendShapeData);
	constantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	result = DX11::Device->CreateBuffer(&constantBufferDesc, nullptr, myBlendShapeBuffer.GetAddressOf());
	if(FAILED(result))
	{
		return false;
	}*/

	myGBufferPS = TextureAssetHandler::GetPixelShader("Shaders\\GBuffer_PS.cso");
	myDeferredVS = TextureAssetHandler::GetVertexShader("Shaders\\Fullscreen_VS.cso");
	myDeferredPS = TextureAssetHandler::GetPixelShader("Shaders\\Deferred_PS.cso");
	myRenderTexPS = TextureAssetHandler::GetPixelShader("Shaders\\RenderTex_PS.cso");

	return true;
}

void DeferredRenderer::GenerateGBuffer(const std::vector<RenderBuffer>& aModelList, float aDeltaTime, float aTotalTime)
{
	if(!Renderer::GetCamera())
	{
		return;
	}


	if(aModelList.empty())
	{
		return;
	}
	for(const RenderBuffer& modelBuffer : aModelList)
	{
		std::shared_ptr<ModelInstance> model = modelBuffer.myModel;
		if(model == nullptr)
		{
			return;
		}

		if(!model->RenderWithDeferred())
		{
			continue;
		}

		HRESULT result = S_FALSE;
		D3D11_MAPPED_SUBRESOURCE bufferData;

		myFrameBufferData.View = Matrix4x4f::GetFastInverse(Renderer::GetViewMatrix());
		myFrameBufferData.CamTranslation = Renderer::GetViewMatrix().GetPosition();
		myFrameBufferData.Projection = Renderer::GetProjectionMatrix();
		myFrameBufferData.RenderMode = static_cast<int>(GraphicsEngine::Get()->GetRenderMode());
		myFrameBufferData.FarPlane = Renderer::GetCamera()->myFarPlane;
		myFrameBufferData.NearPlane = Renderer::GetCamera()->myNearPlane;
		RECT clientRect = DX11::GetClientSize();
		const Vector2ui Resolution = {
					static_cast<unsigned int>(clientRect.right - clientRect.left),
					static_cast<unsigned int>(clientRect.bottom - clientRect.top)
		};
		myFrameBufferData.Resolution = Resolution;

		ZeroMemory(&bufferData, sizeof(D3D11_MAPPED_SUBRESOURCE));
		result = DX11::GetContext()->Map(myFrameBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferData);
		if(FAILED(result))
		{
			// BOOM?
			return;
		}

		memcpy(bufferData.pData, &myFrameBufferData, sizeof(FrameBufferData));
		DX11::GetContext()->Unmap(myFrameBuffer.Get(), 0);

		DX11::GetContext()->VSSetConstantBuffers(0, 1, myFrameBuffer.GetAddressOf());
		DX11::GetContext()->PSSetConstantBuffers(0, 1, myFrameBuffer.GetAddressOf());

		ModelAssetHandler::Get().ResetRenderedModels();



		//bool isInstanced = model->HasRenderedInstance();

		DX11::GetContext()->PSSetShader(myGBufferPS.Get(), nullptr, 0);

		


		myObjectBufferData.IsInstanced = false;
		myObjectBufferData.World = modelBuffer.myTransform;
		myObjectBufferData.WorldInv = Matrix4x4f::GetFastInverse(modelBuffer.myTransform);
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

		result = DX11::GetContext()->Map(myObjectBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferData);
		memcpy(bufferData.pData, &myObjectBufferData, sizeof(ObjectBufferData));
		DX11::GetContext()->Unmap(myObjectBuffer.Get(), 0);


		DX11::GetContext()->VSSetConstantBuffers(1, 1, myObjectBuffer.GetAddressOf());
		DX11::GetContext()->PSSetConstantBuffers(1, 1, myObjectBuffer.GetAddressOf());

		for(int index = 0; index < model->GetNumMeshes(); index++)
		{
			const Model::MeshData& meshData = model->GetMeshData(index);


			//if (!meshData.Blendshapes.empty())
			//{
			//	int numBlendShapes = static_cast<int>(meshData.Blendshapes.size());

			//	// Update blendshape data in constant buffer
			//	BlendShapeData blendShapeData;
			//	blendShapeData.blendShapeCount = numBlendShapes;

			//	// Copy blendshape vertices
			//	for(int i = 0; i < numBlendShapes; i++)
			//	{
			//		for(int j = 0; j < meshData.Blendshapes[i].BlendShapeVertex.size(); j++)
			//		{
			//			if (j + i < 1024)
			//			{
			//				blendShapeData.blendShapeVertices[j + i] = meshData.Blendshapes[i].BlendShapeVertex[j].Position;
			//			}
			//		}
			//	}

			//	// Copy blendshape weights
			//	for(int i = 0; i < numBlendShapes; i++)
			//	{
			//		blendShapeData.blendShapeWeights[i] = meshData.Blendshapes[i].Value;
			//	}

			//	// Map the buffer and copy the data
			//	D3D11_MAPPED_SUBRESOURCE mappedResource;
			//	DX11::GetContext()->Map(myBlendShapeBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
			//	memcpy(mappedResource.pData, &blendShapeData, sizeof(BlendShapeData));
			//	DX11::GetContext()->Unmap(myBlendShapeBuffer.Get(), 0);

			//	DX11::GetContext()->VSSetConstantBuffers(4, 1, myBlendShapeBuffer.GetAddressOf());
			//}

			

			DX11::GetContext()->IASetInputLayout(meshData.myInputLayout.Get());
			DX11::GetContext()->VSSetShader(meshData.myVertexShader.Get(), nullptr, 0);
			DX11::GetContext()->IASetIndexBuffer(meshData.myIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
			DX11::GetContext()->IASetPrimitiveTopology(static_cast<D3D_PRIMITIVE_TOPOLOGY>(meshData.myPrimitiveTopology));

			if(!model->GetMaterial().empty() && static_cast<int>(meshData.myMaterialIndex) < model->GetMaterialSize())
			{
				model->GetMaterial()[meshData.myMaterialIndex]->SetAsResource(nullptr);
			}
			else if(!model->GetMaterial().empty())
			{
				model->GetMaterial()[0]->SetAsResource(nullptr);
			}

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
			else*/
			//{
				DX11::GetContext()->IASetVertexBuffers(0, 1, meshData.myVertexBuffer.GetAddressOf(), &meshData.myStride, &meshData.myOffset);
				DX11::GetContext()->DrawIndexed(meshData.myNumberOfIndices, 0, 0);
			//}

		}

		/*if(isInstanced)
		{
			model->SetHasBeenRenderer(true);
		}*/
	}
}

void DeferredRenderer::Render(Matrix4x4f aView, Matrix4x4f aProjection, const std::shared_ptr<DirectionalLight>& aDirectionalLight,
	const std::shared_ptr<EnvironmentLight>& anEnvironmentLight, std::vector<Light*> aLightList, float aDetlaTime, float aTotalTime)
{
	if(!Renderer::GetCamera())
	{
		return;
	}


	HRESULT result = S_FALSE;
	D3D11_MAPPED_SUBRESOURCE bufferData;




	myFrameBufferData.View = Matrix4x4f::GetFastInverse(aView);
	myFrameBufferData.Projection = aProjection;
	myFrameBufferData.CamTranslation = aView.GetPosition();
	myFrameBufferData.RenderMode = static_cast<int>(GraphicsEngine::Get()->GetRenderMode());
	RECT clientRect = DX11::GetClientSize();
	const Vector2ui Resolution = {
				static_cast<unsigned int>(clientRect.right - clientRect.left),
				static_cast<unsigned int>(clientRect.bottom - clientRect.top)
	};
	myFrameBufferData.Resolution = Resolution;
	myFrameBufferData.FarPlane = Renderer::GetCamera()->myFarPlane;
	myFrameBufferData.NearPlane = Renderer::GetCamera()->myNearPlane;

	ZeroMemory(&bufferData, sizeof(D3D11_MAPPED_SUBRESOURCE));
	result = DX11::GetContext()->Map(myFrameBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferData);
	if(FAILED(result))
	{
		// BOOM?
	}

	memcpy(bufferData.pData, &myFrameBufferData, sizeof(FrameBufferData));
	DX11::GetContext()->Unmap(myFrameBuffer.Get(), 0);

	DX11::GetContext()->VSSetConstantBuffers(0, 1, myFrameBuffer.GetAddressOf());
	DX11::GetContext()->PSSetConstantBuffers(0, 1, myFrameBuffer.GetAddressOf());

	if(anEnvironmentLight)
	{
		anEnvironmentLight->SetAsResource(nullptr);
	}

	if(aDirectionalLight)
	{
		mySceneLightBufferData.DirectionalLight = aDirectionalLight->GetLightBufferData();
		aDirectionalLight->SetAsResource(nullptr);
	}

	mySceneLightBufferData.NumLights = 0;
	ZeroMemory(mySceneLightBufferData.Lights, sizeof(Light::LightBufferData) * MAX_DEFERRED_LIGHTS);

	for(size_t l = 0; l < aLightList.size() && l < MAX_DEFERRED_LIGHTS; l++)
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

	DX11::GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	DX11::GetContext()->IASetInputLayout(nullptr);
	DX11::GetContext()->IASetVertexBuffers(0, 0, nullptr, nullptr, nullptr);
	DX11::GetContext()->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);
	DX11::GetContext()->VSSetShader(myDeferredVS.Get(), nullptr, 0);
	DX11::GetContext()->GSSetShader(nullptr, nullptr, 0);
	DX11::GetContext()->PSSetShader(myDeferredPS.Get(), nullptr, 0);

	DX11::GetContext()->Draw(3, 0);
}

void DeferredRenderer::RenderLate()
{
	DX11::GetContext()->OMSetRenderTargets(1, DX11::BackBuffer.GetAddressOf(), DX11::DepthBuffer.Get());
	DX11::GetContext()->PSSetShaderResources(0, 1, DX11::RenderSRV.GetAddressOf());
	DX11::GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	DX11::GetContext()->IASetInputLayout(nullptr);
	DX11::GetContext()->IAGetVertexBuffers(0, 0, nullptr, nullptr, nullptr);
	DX11::GetContext()->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);
	DX11::GetContext()->VSSetShader(myDeferredVS.Get(), nullptr, 0);
	DX11::GetContext()->GSSetShader(nullptr, nullptr, 0);
	DX11::GetContext()->PSSetShader(myRenderTexPS.Get(), nullptr, 0);

	DX11::GetContext()->Draw(3, 0);
}

void DeferredRenderer::ClearTarget()
{
	ID3D11ShaderResourceView* srvList[1];
	for(int i = 0; i < 1; i++)
	{
		srvList[i] = nullptr;
	}
	DX11::GetContext()->PSSetShaderResources(0, 1, &srvList[0]);
	DX11::GetContext()->VSSetShaderResources(0, 1, &srvList[0]);
}
