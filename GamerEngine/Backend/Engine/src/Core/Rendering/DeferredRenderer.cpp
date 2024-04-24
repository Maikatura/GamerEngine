#include <GraphicsEngine.pch.h>
#include "DeferredRenderer.h"
#include "ForwardRenderer.h"
#include "GraphicsEngine.h"
#include "Renderer.h"
#include "AssetHandlers/ModelAssetHandler.h"
#include "Components/TransfromComponent.h"
#include "Core/Framework/DX11.h"

void GBuffer::SetAsTarget()
{
	ID3D11RenderTargetView* rtvList[GBufferTexture::EGBufferTexture_Count];
	for(size_t i = 0; i < myRenderTextures.size(); i++)
	{
		rtvList[i] = myRenderTextures[i].GetRenderTargetView();
		
	}

	DX11::Get().GetContext()->OMSetRenderTargets(GBufferTexture::EGBufferTexture_Count, &rtvList[0], DX11::Get().GetDepthStencilView()->myDSV.Get());
}

void GBuffer::ClearTarget()
{
	DX11::Get().ResetRenderTarget(GraphicsEngine::Get()->GetEditorMode());
}


void GBuffer::SetAsResource(unsigned int aStartSlot)
{
	ID3D11ShaderResourceView* mySRVList[GBufferTexture::EGBufferTexture_Count];
	for(int t = 0; t < GBufferTexture::EGBufferTexture_Count; t++)
	{
		mySRVList[t] = myRenderTextures[t].GetShaderResourceView();
	}

	//DX11::Get().GetContext()->VSSetShaderResources(aStartSlot, GBufferTexture::GBufferTexture_Count, &mySRVList[0]);
	DX11::Get().GetContext()->PSSetShaderResources(aStartSlot, GBufferTexture::EGBufferTexture_Count, &mySRVList[0]);
	
}

void GBuffer::ClearResource(unsigned int aStartSlot)
{

	ID3D11ShaderResourceView* srvList[GBufferTexture::EGBufferTexture_Count];
	for(int i = 0; i < GBufferTexture::EGBufferTexture_Count; i++)
	{
		srvList[i] = nullptr;
		myRenderTextures[i].ClearRenderTarget(DX11::Get().GetContext(),nullptr, 0, 0, 0, 0);
	}


	//DX11::Get().GetContext()->VSSetShaderResources(aStartSlot, GBufferTexture::GBufferTexture_Count, &srvList[0]);
	DX11::Get().GetContext()->PSSetShaderResources(aStartSlot, GBufferTexture::EGBufferTexture_Count, &srvList[0]);
}

void GBuffer::Clear()
{
	const Vector4f clCol = { 0, 0, 0, 0 };
	for(int i = 0; i < GBufferTexture::EGBufferTexture_Count; i++)
	{
		myRenderTextures[i].ClearRenderTarget(DX11::Get().GetContext(), nullptr, clCol.x, clCol.y, clCol.z, clCol.w);
	}
}

void GBuffer::Release()
{
	for (auto& myRenderTexture : myRenderTextures)
	{
		myRenderTexture.Shutdown();
	}
}

bool GBuffer::CreateGBuffer()
{
	const Rect clientRect = DX11::Get().GetClientSize();
	const int width = static_cast<int>(clientRect.Right - clientRect.Left);
	const int height = static_cast<int>(clientRect.Bottom - clientRect.Top);
	
	myRenderTextures[GBuffer::GBufferTexture::EGBufferTexture_Albedo].Initialize(DX11::Get().GetDevice(), width, height, DXGI_FORMAT_R8G8B8A8_UNORM, "Albedo GBuffer");

	myRenderTextures[GBuffer::GBufferTexture::EGBufferTexture_Normal].Initialize(DX11::Get().GetDevice(), width, height, DXGI_FORMAT_R16G16B16A16_SNORM, "Normal GBuffer");

	myRenderTextures[GBuffer::GBufferTexture::EGBufferTexture_Material].Initialize(DX11::Get().GetDevice(), width, height, DXGI_FORMAT_R16G16B16A16_SNORM, "Material GBuffer");

	myRenderTextures[GBuffer::GBufferTexture::EGBufferTexture_VertexNormal].Initialize(DX11::Get().GetDevice(), width, height, DXGI_FORMAT_R16G16B16A16_SNORM, "Vertex Normal GBuffer");

	myRenderTextures[GBuffer::GBufferTexture::EGBufferTexture_Position].Initialize(DX11::Get().GetDevice(), width, height, DXGI_FORMAT_R32G32B32A32_FLOAT, "Position GBuffer");

	myRenderTextures[GBuffer::GBufferTexture::EGBufferTexture_AmbientOcclusion].Initialize(DX11::Get().GetDevice(), width, height, DXGI_FORMAT_R8_UNORM, "AO GBuffer");

	myRenderTextures[GBuffer::GBufferTexture::EGBufferTexture_ViewPosition].Initialize(DX11::Get().GetDevice(), width, height, DXGI_FORMAT_R32G32B32A32_FLOAT, "View Position GBuffer");

	myRenderTextures[GBuffer::GBufferTexture::EGBufferTexture_ViewNormal].Initialize(DX11::Get().GetDevice(), width, height, DXGI_FORMAT_R16G16B16A16_SNORM, "View Normal GBuffer");

	myRenderer.Initialize(DX11::Get().GetDevice(), width, height);

	return true;
}

RenderTexture& GBuffer::GetRenderer()
{
	return myRenderer;
}

std::array<RenderTexture, GBuffer::EGBufferTexture_Count>& GBuffer::GetPasses()
{
	return myRenderTextures;
}

bool DeferredRenderer::Initialize()
{
	mySceneLightBufferData = {};

	
	D3D11_BUFFER_DESC bufferDescription;
	ZeroMemory(&bufferDescription, sizeof(D3D11_BUFFER_DESC));
	bufferDescription.Usage = D3D11_USAGE_DYNAMIC;
	bufferDescription.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	bufferDescription.ByteWidth = sizeof(FrameBufferData);
	HRESULT result = DX11::Get().GetDevice()->CreateBuffer(&bufferDescription, nullptr, myFrameBuffer.GetAddressOf());
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

	bufferDescription.ByteWidth = sizeof(SceneLightBuffer);
	result = DX11::Get().GetDevice()->CreateBuffer(&bufferDescription, nullptr, myLightBuffer.GetAddressOf());
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

	result = DX11::Get().Device->CreateBuffer(&constantBufferDesc, nullptr, myBlendShapeBuffer.GetAddressOf());
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

void DeferredRenderer::GenerateGBuffer(Matrix4x4f aView, const Matrix4x4f& aProjection, const std::vector<RenderBuffer>& aModelList, float aDeltaTime, float aTotalTime, VREye anEye)
{
	const auto* camera = Renderer::GetCamera();
	if(!camera)
	{
		return;
	}


	ModelAssetHandler::Get().ResetRenderedModels();

	if(aModelList.empty())
	{
		return;
	}
	
	D3D11_MAPPED_SUBRESOURCE bufferData;

	myFrameBufferData.View = Matrix4x4f::GetFastInverse(aView);
	myFrameBufferData.CamTranslation = aView.GetPosition();
	myFrameBufferData.Projection = aProjection;
	myFrameBufferData.RenderMode = static_cast<int>(GraphicsEngine::Get()->GetRenderMode());
	myFrameBufferData.FarPlane = camera->myFarPlane;
	myFrameBufferData.NearPlane = camera->myNearPlane;


	const Rect clientRect = DX11::Get().GetClientSize();
	const uint32_t width = anEye == VREye::None ? clientRect.Right - clientRect.Left : DX11::Get().GetScreenSize().x;
	const uint32_t height = anEye == VREye::None ? clientRect.Bottom - clientRect.Top : DX11::Get().GetScreenSize().y;

	const Vector2ui Resolution = {
		width,
		height
	};
	myFrameBufferData.Resolution = Resolution;


	ZeroMemory(&bufferData, sizeof(D3D11_MAPPED_SUBRESOURCE));
	HRESULT result = DX11::Get().GetContext()->Map(myFrameBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferData);
	if (FAILED(result))
	{
		// BOOM?
		return;
	}

	memcpy_s(bufferData.pData,sizeof(FrameBufferData), &myFrameBufferData, sizeof(FrameBufferData));
	DX11::Get().GetContext()->Unmap(myFrameBuffer.Get(), 0);

	DX11::Get().GetContext()->VSSetConstantBuffers(0, 1, myFrameBuffer.GetAddressOf());
	DX11::Get().GetContext()->PSSetConstantBuffers(0, 1, myFrameBuffer.GetAddressOf());

	for(const RenderBuffer& modelBuffer : aModelList)
	{
		Ref<GamerEngine::Model> model = modelBuffer.myModel;
		if(model == nullptr)
		{
			return;
		}

		if(!model->UseDeferred())
		{
			continue;
		}


		const bool isInstanced = model->HasRenderedInstance();

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
				bones.data(), sizeof(Matrix4x4f) * MAX_MODEL_BONES
			);
		}

		result = DX11::Get().GetContext()->Map(myObjectBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferData);
		if (FAILED(result))
		{
			// BOOM?
			return;
		}
		memcpy_s(bufferData.pData,sizeof(ObjectBufferData), &myObjectBufferData, sizeof(ObjectBufferData));
		DX11::Get().GetContext()->Unmap(myObjectBuffer.Get(), 0);


		DX11::Get().GetContext()->VSSetConstantBuffers(1, 1, myObjectBuffer.GetAddressOf());
		DX11::Get().GetContext()->PSSetConstantBuffers(1, 1, myObjectBuffer.GetAddressOf());

		DX11::Get().GetContext()->PSSetShader(myGBufferPS.Get(), nullptr, 0);

		for(size_t index = 0; index < model->GetNumMeshes(); index++)
		{
			GamerEngine::Model::MeshData& meshData = model->GetMeshData(static_cast<unsigned>(index));
			meshData.MaterialData.SetAsResource(myMaterialBuffer);
			DX11::Get().GetContext()->PSSetConstantBuffers(2, 1, myMaterialBuffer.GetAddressOf());
			DX11::Get().GetContext()->VSSetConstantBuffers(2, 1, myMaterialBuffer.GetAddressOf());

			
			DX11::Get().GetContext()->IASetInputLayout(meshData.myVertexShader->GetInputLayout().Get());
			DX11::Get().GetContext()->VSSetShader(meshData.myVertexShader->Get().Get(), nullptr, 0);
			DX11::Get().GetContext()->IASetIndexBuffer(meshData.myIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
			DX11::Get().GetContext()->IASetPrimitiveTopology(static_cast<D3D_PRIMITIVE_TOPOLOGY>(meshData.myPrimitiveTopology));

			if(isInstanced)
			{
				myInstancedTransformBufferData.clear();
				std::vector<GamerEngine::Model::RenderedInstanceData> myTransformData = model->GetTransformVector();
				for(int i = 0; i < myTransformData.size(); i++)
				{
					auto matrix = myTransformData[i].World;
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
			{
				DX11::Get().GetContext()->IASetVertexBuffers(0, 1, meshData.myVertexBuffer.GetAddressOf(), &meshData.myStride, &meshData.myOffset);
				DX11::Get().GetContext()->DrawIndexed(meshData.myNumberOfIndices, 0, 0);
			}

		}

		//model->ClearInstanceData();

		if(isInstanced)
		{
			//model->SetHasBeenRenderer(true);
			//model->ClearInstanceData();
			
		}
	}
}

void DeferredRenderer::Render(Matrix4x4f aView, const Matrix4x4f& aProjection, const Ref<DirectionalLight>& aDirectionalLight,
	const Ref<EnvironmentLight>& anEnvironmentLight, const std::vector<Light*>& aLightList, float aDeltaTime, float aTotalTime, VREye anEye)
{
	const auto* camera = Renderer::GetCamera();
	if(!camera)
	{
		return;
	}

	D3D11_MAPPED_SUBRESOURCE bufferData;
	
	myFrameBufferData.View = Matrix4x4f::GetFastInverse(aView);
	myFrameBufferData.CamTranslation = aView.GetPosition();
	myFrameBufferData.Projection = aProjection;
	myFrameBufferData.RenderMode = static_cast<int>(GraphicsEngine::Get()->GetRenderMode());
	myFrameBufferData.FarPlane = camera->myFarPlane;
	myFrameBufferData.NearPlane = camera->myNearPlane;

	const Rect clientRect = DX11::Get().GetClientSize();
	const uint32_t width = anEye == VREye::None ? clientRect.Right - clientRect.Left : DX11::Get().GetScreenSize().x;
	const uint32_t height = anEye == VREye::None ? clientRect.Bottom - clientRect.Top : DX11::Get().GetScreenSize().y;

	const Vector2ui Resolution = {
		width,
		height
	};
	myFrameBufferData.Resolution = Resolution;

	ZeroMemory(&bufferData, sizeof(D3D11_MAPPED_SUBRESOURCE));
	HRESULT result = result = DX11::Get().GetContext()->Map(myFrameBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferData);
	if (SUCCEEDED(result))
	{
		memcpy_s(bufferData.pData,sizeof(FrameBufferData), &myFrameBufferData, sizeof(FrameBufferData));
		DX11::Get().GetContext()->Unmap(myFrameBuffer.Get(), 0);
	}
	else
	{
		GE_LOG_ERROR("Failed to move over to the Frame Buffer (Deferred Renderer)")
	}
	
	DX11::Get().GetContext()->VSSetConstantBuffers(0, 1, myFrameBuffer.GetAddressOf());
	DX11::Get().GetContext()->PSSetConstantBuffers(0, 1, myFrameBuffer.GetAddressOf());

	if(anEnvironmentLight)
	{
		anEnvironmentLight->SetAsResource(nullptr, 0);
	}

	if(aDirectionalLight)
	{
		mySceneLightBufferData.DirectionalLight = aDirectionalLight->GetLightBufferData();
		aDirectionalLight->SetAsResource(nullptr, 0);
	}

	mySceneLightBufferData.NumLightsPoint = 0;
	mySceneLightBufferData.NumLightsSpot = 0;
	ZeroMemory(mySceneLightBufferData.LightsPoint, sizeof(Light::LightBufferData) * MAX_DEFERRED_LIGHTS);
	ZeroMemory(mySceneLightBufferData.LightsSpot, sizeof(Light::LightBufferData) * MAX_DEFERRED_LIGHTS);

	for (const auto light : aLightList)
	{
		if (light->GetLightBufferData().LightType == 1)
		{
			continue;
		}
		else
		{
			if (light->GetLightBufferData().LightType == 2 && mySceneLightBufferData.NumLightsPoint < MAX_FORWARD_LIGHTS)
			{
				mySceneLightBufferData.LightsPoint[mySceneLightBufferData.NumLightsPoint] = light->GetLightBufferData();
				light->SetAsResource(nullptr, mySceneLightBufferData.NumLightsPoint);
				mySceneLightBufferData.NumLightsPoint++;

			}

			if (light->GetLightBufferData().LightType == 3 && mySceneLightBufferData.NumLightsSpot < MAX_FORWARD_LIGHTS)
			{
				mySceneLightBufferData.LightsSpot[mySceneLightBufferData.NumLightsSpot] = light->GetLightBufferData();
				light->SetAsResource(nullptr, mySceneLightBufferData.NumLightsSpot);
				mySceneLightBufferData.NumLightsSpot++;
			}
		}
	}

	result = DX11::Get().GetContext()->Map(
		myLightBuffer.Get(),
		0,
		D3D11_MAP_WRITE_DISCARD,
		0,
		&bufferData);

	if (SUCCEEDED(result))
	{
		memcpy_s(bufferData.pData,sizeof(SceneLightBuffer), &mySceneLightBufferData, sizeof(SceneLightBuffer));
		DX11::Get().GetContext()->Unmap(myLightBuffer.Get(), 0);
	}
	else
	{
		GE_LOG_ERROR("Failed to move over to the Scene Light Buffer (Deferred Renderer)")
	}

	
	ZeroMemory(&bufferData, sizeof(D3D11_MAPPED_SUBRESOURCE));
	result = DX11::Get().GetContext()->Map(
		myMaterialBuffer.Get(),
		0,
		D3D11_MAP_WRITE_DISCARD,
		0,
		&bufferData
	);
	if (FAILED(result))
	{
		// ?? what happen?????????
	}

	const Vector4f MaterialColor = { 1,1,1,1 };
	memcpy(bufferData.pData, &MaterialColor, sizeof(Vector4f));
	DX11::Get().GetContext()->Unmap(myMaterialBuffer.Get(), 0);


	DX11::Get().GetContext()->PSSetConstantBuffers(3, 1, myLightBuffer.GetAddressOf());

	DX11::Get().GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	DX11::Get().GetContext()->IASetInputLayout(nullptr);
	DX11::Get().GetContext()->IASetVertexBuffers(0, 0, nullptr, nullptr, nullptr);
	DX11::Get().GetContext()->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);
	DX11::Get().GetContext()->VSSetShader(myDeferredVS.Get(), nullptr, 0);
	DX11::Get().GetContext()->GSSetShader(nullptr, nullptr, 0);
	DX11::Get().GetContext()->PSSetShader(myDeferredPS.Get(), nullptr, 0);

	DX11::Get().GetContext()->Draw(3, 0);
}

void DeferredRenderer::RenderLate() const
{
	const auto srv = GBuffer::GetRenderer().GetShaderResourceView();
	DX11::Get().GetContext()->PSSetShaderResources(0, 1, &srv);
	DX11::Get().GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	DX11::Get().GetContext()->IASetInputLayout(nullptr);
	DX11::Get().GetContext()->IAGetVertexBuffers(0, 0, nullptr, nullptr, nullptr);
	DX11::Get().GetContext()->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);
	DX11::Get().GetContext()->VSSetShader(myDeferredVS.Get(), nullptr, 0);
	DX11::Get().GetContext()->GSSetShader(nullptr, nullptr, 0);
	DX11::Get().GetContext()->PSSetShader(myRenderTexPS.Get(), nullptr, 0);

	DX11::Get().GetContext()->Draw(3, 0);

	//DX11::Get().GetContext()->PSSetShaderResources(0, 1, nullptr);
}

void DeferredRenderer::ClearTarget()
{
	ID3D11ShaderResourceView* srvList[1];
	for (auto& i : srvList)
	{
		i = nullptr;
	}
	DX11::Get().GetContext()->PSSetShaderResources(0, 1, &srvList[0]);
	DX11::Get().GetContext()->VSSetShaderResources(0, 1, &srvList[0]);
}
