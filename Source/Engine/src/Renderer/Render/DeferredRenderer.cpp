#include <GraphicsEngine.pch.h>
#include <Renderer/Render/DeferredRenderer.h>

#include "ForwardRenderer.h"
#include "Renderer/GraphicsEngine.h"
#include "Renderer.h"
#include "Renderer/AssetHandlers/ModelAssetHandler.h"
#include "Renderer/Framework/DX11.h"

void GBuffer::SetAsTarget() const
{
	ID3D11RenderTargetView* rtvList[GBufferTexture::GB_COUNT];
	for(int i = 0; i < myRTVs.size(); i++)
	{
		rtvList[i] = myRTVs[i].Get();
	}
	//rtvList[ID] = DX11::ourIDBuffer.Get();
	DX11::Context->OMSetRenderTargets(GBufferTexture::GB_COUNT, &rtvList[0], DX11::DepthBuffer.Get());
}

void GBuffer::ClearTarget() const
{
	ID3D11RenderTargetView* rtvList[GBufferTexture::GB_COUNT];
	for(int i = 0; i < GBufferTexture::GB_COUNT; i++)
	{
		rtvList[i] = nullptr;
		/*Vector4f color = { 0,0,0,0 };
		DX11::Context->ClearRenderTargetView(myRTVs[i].Get(), &color.x);*/
	}
	DX11::Context->OMSetRenderTargets(1, DX11::RenderRTV.GetAddressOf(), DX11::DepthBuffer.Get());
}

void GBuffer::SetAsResource(unsigned aStartSlot) const
{
	ID3D11ShaderResourceView* mySRVList[GBufferTexture::GB_COUNT];
	for(int t = 0; t < GBufferTexture::GB_COUNT; t++)
	{
		mySRVList[t] = mySRVs[t].Get();
	}

	DX11::Context->VSSetShaderResources(aStartSlot, GBufferTexture::GB_COUNT, &mySRVList[0]);
	DX11::Context->PSSetShaderResources(aStartSlot, GBufferTexture::GB_COUNT, &mySRVList[0]);
}

void GBuffer::ClearResource(unsigned aStartSlot) const
{

	ID3D11ShaderResourceView* srvList[GBufferTexture::GB_COUNT];
	for(int i = 0; i < GBufferTexture::GB_COUNT; i++)
	{
		srvList[i] = nullptr;
	}
	DX11::Context->VSSetShaderResources(aStartSlot, GBufferTexture::GB_COUNT, &srvList[0]);
	DX11::Context->PSSetShaderResources(aStartSlot, GBufferTexture::GB_COUNT, &srvList[0]);
}

void GBuffer::Clear()
{
	Vector4f clCol = { 0, 0, 0, 0 };
	for(int i = 0; i < GBufferTexture::GB_COUNT; i++)
	{
		DX11::Context->ClearRenderTargetView(myRTVs[i].Get(), &clCol.x);
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
	bufferDesc.SampleDesc = sampleDesc;
	bufferDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	bufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	bufferDesc.CPUAccessFlags = 0;

	result = DX11::Device->CreateTexture2D(&bufferDesc, nullptr, &myTexture);
	if(FAILED(result))
		return false;
	result = DX11::Device->CreateRenderTargetView(myTexture, nullptr, myRTVs[GBuffer::GBufferTexture::GB_ALBEDO].ReleaseAndGetAddressOf());
	if(FAILED(result))
		return false;
	result = DX11::Device->CreateShaderResourceView(myTexture, nullptr, mySRVs[GBuffer::GBufferTexture::GB_ALBEDO].ReleaseAndGetAddressOf());
	if(FAILED(result))
		return false;


	bufferDesc.Format = DXGI_FORMAT_R16G16B16A16_SNORM;

	result = DX11::Device->CreateTexture2D(&bufferDesc, nullptr, &myTexture);
	if(FAILED(result))
		return false;
	result = DX11::Device->CreateRenderTargetView(myTexture, nullptr, myRTVs[GBuffer::GBufferTexture::GB_NORMAL].ReleaseAndGetAddressOf());
	if(FAILED(result))
		return false;
	result = DX11::Device->CreateShaderResourceView(myTexture, nullptr, mySRVs[GBuffer::GBufferTexture::GB_NORMAL].ReleaseAndGetAddressOf());
	if(FAILED(result))
		return false;


	bufferDesc.Format = DXGI_FORMAT_R16G16B16A16_SNORM;

	result = DX11::Device->CreateTexture2D(&bufferDesc, nullptr, &myTexture);
	if(FAILED(result))
		return false;
	result = DX11::Device->CreateRenderTargetView(myTexture, nullptr, myRTVs[GBuffer::GBufferTexture::GB_MATERIAL].ReleaseAndGetAddressOf());
	if(FAILED(result))
		return false;
	result = DX11::Device->CreateShaderResourceView(myTexture, nullptr, mySRVs[GBuffer::GBufferTexture::GB_MATERIAL].ReleaseAndGetAddressOf());
	if(FAILED(result))
		return false;



	bufferDesc.Format = DXGI_FORMAT_R16G16B16A16_SNORM;

	result = DX11::Device->CreateTexture2D(&bufferDesc, nullptr, &myTexture);
	if(FAILED(result))
		return false;
	result = DX11::Device->CreateRenderTargetView(myTexture, nullptr, myRTVs[GBuffer::GBufferTexture::GB_VERTEXNORMAL].ReleaseAndGetAddressOf());
	if(FAILED(result))
		return false;
	result = DX11::Device->CreateShaderResourceView(myTexture, nullptr, mySRVs[GBuffer::GBufferTexture::GB_VERTEXNORMAL].ReleaseAndGetAddressOf());
	if(FAILED(result))
		return false;



	bufferDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	result = DX11::Device->CreateTexture2D(&bufferDesc, nullptr, &myTexture);
	if(FAILED(result))
		return false;
	result = DX11::Device->CreateRenderTargetView(myTexture, nullptr, myRTVs[GBuffer::GBufferTexture::GB_POSITION].ReleaseAndGetAddressOf());
	if(FAILED(result))
		return false;
	result = DX11::Device->CreateShaderResourceView(myTexture, nullptr, mySRVs[GBuffer::GBufferTexture::GB_POSITION].ReleaseAndGetAddressOf());
	if(FAILED(result))
		return false;


	bufferDesc.Format = DXGI_FORMAT_R8_UNORM;
	result = DX11::Device->CreateTexture2D(&bufferDesc, nullptr, &myTexture);
	if(FAILED(result))
		return false;
	result = DX11::Device->CreateRenderTargetView(myTexture, nullptr, myRTVs[GBuffer::GBufferTexture::GB_AMBIENTOCCLUSION].ReleaseAndGetAddressOf());
	if(FAILED(result))
		return false;
	result = DX11::Device->CreateShaderResourceView(myTexture, nullptr, mySRVs[GBuffer::GBufferTexture::GB_AMBIENTOCCLUSION].ReleaseAndGetAddressOf());
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


	myGBufferPS = TextureAssetHandler::GetPixelShader("Shaders\\GBuffer_PS.cso");
	myDeferredVS = TextureAssetHandler::GetVertexShader("Shaders\\Fullscreen_VS.cso");
	myDeferredPS = TextureAssetHandler::GetPixelShader("Shaders\\Deferred_PS.cso");
	myRenderTexPS = TextureAssetHandler::GetPixelShader("Shaders\\RenderTex_PS.cso");

	return true;
}

void DeferredRenderer::GenerateGBuffer(const std::vector<RenderBuffer>& aModelList, float aDeltaTime, float aTotalTime)
{
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
		DX11::Context->PSSetConstantBuffers(0, 1, myFrameBuffer.GetAddressOf());

		ModelAssetHandler::ResetRenderedModels();



		bool isInstanced = model->HasRenderedInstance();

		DX11::Context->PSSetShader(myGBufferPS.Get(), nullptr, 0);

		myObjectBufferData.IsInstanced = isInstanced;
		myObjectBufferData.World = modelBuffer.myTransform;
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
		memcpy(bufferData.pData, &myObjectBufferData, sizeof(ObjectBufferData));
		DX11::Context->Unmap(myObjectBuffer.Get(), 0);



		DX11::Context->VSSetConstantBuffers(1, 1, myObjectBuffer.GetAddressOf());
		DX11::Context->PSSetConstantBuffers(1, 1, myObjectBuffer.GetAddressOf());

		bool finised = false;

		for(int index = 0; index < model->GetNumMeshes(); index++)
		{
			const Model::MeshData& meshData = model->GetMeshData(index);

			DX11::Context->IASetInputLayout(meshData.myInputLayout.Get());
			DX11::Context->VSSetShader(meshData.myVertexShader.Get(), nullptr, 0);
			DX11::Context->IASetIndexBuffer(meshData.myIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
			DX11::Context->IASetPrimitiveTopology(static_cast<D3D_PRIMITIVE_TOPOLOGY>(meshData.myPrimitiveTopology));

			if(model->GetMaterial())
			{
				model->GetMaterial()->SetAsResource(nullptr);
			}
			else
			{

			}

			if(isInstanced && !model->HasBeenRendered())
			{
				myInstancedTransformBufferData.clear();
				std::vector<ModelInstance::RenderedInstanceData> myTransformData = model->GetTransformVector();
				for(int i = 0; i < myTransformData.size(); i++)
				{
					auto matrix = myTransformData[i].World->GetMatrix();
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
			else
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

void DeferredRenderer::Render(const std::shared_ptr<DirectionalLight>& aDirectionalLight,
	const std::shared_ptr<EnvironmentLight>& anEnvironmentLight, std::vector<Light*> aLightList, float aDetlaTime, float aTotalTime)
{
	HRESULT result = S_FALSE;
	D3D11_MAPPED_SUBRESOURCE bufferData;

	myFrameBufferData.View = Matrix4x4f::GetFastInverse(Renderer::GetViewMatrix());
	myFrameBufferData.Projection = Renderer::GetProjectionMatrix();
	myFrameBufferData.CamTranslation = Renderer::GetViewMatrix().GetPosition();
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
	result = DX11::Context->Map(myFrameBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferData);
	if(FAILED(result))
	{
		// BOOM?
	}

	memcpy(bufferData.pData, &myFrameBufferData, sizeof(FrameBufferData));
	DX11::Context->Unmap(myFrameBuffer.Get(), 0);

	DX11::Context->VSSetConstantBuffers(0, 1, myFrameBuffer.GetAddressOf());
	DX11::Context->PSSetConstantBuffers(0, 1, myFrameBuffer.GetAddressOf());

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

	DX11::Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	DX11::Context->IASetInputLayout(nullptr);
	DX11::Context->IASetVertexBuffers(0, 0, nullptr, nullptr, nullptr);
	DX11::Context->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);
	DX11::Context->VSSetShader(myDeferredVS.Get(), nullptr, 0);
	DX11::Context->GSSetShader(nullptr, nullptr, 0);
	DX11::Context->PSSetShader(myDeferredPS.Get(), nullptr, 0);

	DX11::Context->Draw(3, 0);
}

void DeferredRenderer::RenderLate()
{
	DX11::Context->OMSetRenderTargets(1, DX11::BackBuffer.GetAddressOf(), DX11::DepthBuffer.Get());
	DX11::Context->PSSetShaderResources(0, 1, DX11::RenderSRV.GetAddressOf());
	DX11::Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	DX11::Context->IASetInputLayout(nullptr);
	DX11::Context->IAGetVertexBuffers(0, 0, nullptr, nullptr, nullptr);
	DX11::Context->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);
	DX11::Context->VSSetShader(myDeferredVS.Get(), nullptr, 0);
	DX11::Context->GSSetShader(nullptr, nullptr, 0);
	DX11::Context->PSSetShader(myRenderTexPS.Get(), nullptr, 0);

	DX11::Context->Draw(3, 0);
}

void DeferredRenderer::ClearTarget()
{
	ID3D11ShaderResourceView* srvList[1];
	for(int i = 0; i < 1; i++)
	{
		srvList[i] = nullptr;
	}
	DX11::Context->PSSetShaderResources(0, 1, &srvList[0]);
	DX11::Context->VSSetShaderResources(0, 1, &srvList[0]);
}
