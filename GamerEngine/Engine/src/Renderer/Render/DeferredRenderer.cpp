#include <GraphicsEngine.pch.h>
#include <Renderer/Render/DeferredRenderer.h>

#include "ForwardRenderer.h"
#include "Renderer/GraphicsEngine.h"
#include "Renderer.h"
#include "Renderer/AssetHandlers/ModelAssetHandler.h"
#include "Renderer/Framework/DX11.h"

void GBuffer::SetAsTarget()
{
	ID3D11RenderTargetView* rtvList[GBufferTexture::GBufferTexture_Count];
	for(int i = 0; i < myRenderTextures.size(); i++)
	{
		rtvList[i] = myRenderTextures[i].GetRenderTargetView();
		
	}

	//rtvList[ID] = DX11::ourIDBuffer.Get();

	DX11::GetContext()->OMSetRenderTargets(GBufferTexture::GBufferTexture_Count, &rtvList[0], DX11::GetDepthStencilView());
}

void GBuffer::ClearTarget()
{
	Vector4f color = { 0,0,1.0f,0 };
	ID3D11RenderTargetView* rtvList[GBufferTexture::GBufferTexture_Count];
	for(int i = 0; i < GBufferTexture::GBufferTexture_Count; i++)
	{
		rtvList[i] = nullptr;

		myRenderTextures[i].ClearRenderTarget(DX11::GetContext(), DX11::GetDepthStencilView(), color.x, color.y, color.z, color.w);
		//DX11::GetContext()->ClearRenderTargetView(myRTVs[i].Get(), &color.x);
	}

	myRenderer.ClearRenderTarget(DX11::GetContext(), DX11::GetDepthStencilView(), color.x, color.y, color.z, color.w);
	myRenderer.SetRenderTarget(DX11::GetContext(), DX11::GetDepthStencilView());
}


void GBuffer::SetAsResource(unsigned aStartSlot)
{
	ID3D11ShaderResourceView* mySRVList[GBufferTexture::GBufferTexture_Count];
	for(int t = 0; t < GBufferTexture::GBufferTexture_Count; t++)
	{
		mySRVList[t] = myRenderTextures[t].GetShaderResourceView();
	}

	DX11::GetContext()->VSSetShaderResources(aStartSlot, GBufferTexture::GBufferTexture_Count, &mySRVList[0]);
	DX11::GetContext()->PSSetShaderResources(aStartSlot, GBufferTexture::GBufferTexture_Count, &mySRVList[0]);
	
}

void GBuffer::ClearResource(unsigned aStartSlot)
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
		myRenderTextures[i].ClearRenderTarget(DX11::GetContext(), nullptr, clCol.x, clCol.y, clCol.z, clCol.w);
	}
}

void GBuffer::Release()
{
	for(unsigned t = 0; t < myRenderTextures.size(); t++)
	{
		myRenderTextures[t].Shutdown();
	}
}

bool GBuffer::CreateGBuffer()
{

	
	RECT clientRect = DX11::GetClientSize();

	int width = static_cast<UINT>(clientRect.right - clientRect.left);
	int height = static_cast<UINT>(clientRect.bottom - clientRect.top);

	

	myRenderTextures[GBuffer::GBufferTexture::GBufferTexture_Albedo].Initialize(DX11::GetDevice(), width, height, DXGI_FORMAT_R8G8B8A8_UNORM);

	myRenderTextures[GBuffer::GBufferTexture::GBufferTexture_Normal].Initialize(DX11::GetDevice(), width, height, DXGI_FORMAT_R16G16B16A16_SNORM);

	myRenderTextures[GBuffer::GBufferTexture::GBufferTexture_Material].Initialize(DX11::GetDevice(), width, height, DXGI_FORMAT_R16G16B16A16_SNORM);

	myRenderTextures[GBuffer::GBufferTexture::GBufferTexture_VertexNormal].Initialize(DX11::GetDevice(), width, height, DXGI_FORMAT_R16G16B16A16_SNORM);

	myRenderTextures[GBuffer::GBufferTexture::GBufferTexture_Position].Initialize(DX11::GetDevice(), width, height, DXGI_FORMAT_R32G32B32A32_FLOAT);

	myRenderTextures[GBuffer::GBufferTexture::GBufferTexture_AmbientOcclusion].Initialize(DX11::GetDevice(), width, height, DXGI_FORMAT_R8_UNORM);

	myRenderTextures[GBuffer::GBufferTexture::GBufferTexture_ViewPosition].Initialize(DX11::GetDevice(), width, height, DXGI_FORMAT_R32G32B32A32_FLOAT);

	myRenderTextures[GBuffer::GBufferTexture::GBufferTexture_ViewNormal].Initialize(DX11::GetDevice(), width, height, DXGI_FORMAT_R16G16B16A16_SNORM);

	myRenderer.Initialize(DX11::GetDevice(), width, height);

	return true;
}

RenderTexture& GBuffer::GetRenderer()
{
	return myRenderer;
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

void DeferredRenderer::GenerateGBuffer(Matrix4x4f aView, Matrix4x4f aProjection, const std::vector<RenderBuffer>& aModelList, float aDeltaTime, float aTotalTime, VREye anEye)
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

		if(!model->RenderWithDeferred())
		{
			continue;
		}

		

		

		ModelAssetHandler::Get().ResetRenderedModels();


		bool isInstanced = false; //model->HasRenderedInstance();


		myObjectBufferData.IsInstanced = isInstanced;
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

		DX11::GetContext()->PSSetShader(myGBufferPS.Get(), nullptr, 0);

		for(int index = 0; index < model->GetNumMeshes(); index++)
		{
			const Model::MeshData& meshData = model->GetMeshData(index);


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
	const std::shared_ptr<EnvironmentLight>& anEnvironmentLight, std::vector<Light*> aLightList, float aDetlaTime, float aTotalTime, VREye anEye)
{
	if(!Renderer::GetCamera())
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

	auto srv = GBuffer::GetRenderer().GetShaderResourceView();
	DX11::GetContext()->PSSetShaderResources(0, 1, &srv);
	DX11::GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	DX11::GetContext()->IASetInputLayout(nullptr);
	DX11::GetContext()->IAGetVertexBuffers(0, 0, nullptr, nullptr, nullptr);
	DX11::GetContext()->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);
	DX11::GetContext()->VSSetShader(myDeferredVS.Get(), nullptr, 0);
	DX11::GetContext()->GSSetShader(nullptr, nullptr, 0);
	DX11::GetContext()->PSSetShader(myRenderTexPS.Get(), nullptr, 0);

	DX11::GetContext()->Draw(3, 0);

	//DX11::GetContext()->PSSetShaderResources(0, 1, nullptr);
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
