#include <GraphicsEngine.pch.h>
#include <Render/DeferredRenderer.h>

#include "ForwardRenderer.h"
#include "GraphicsEngine.h"
#include "Renderer.h"
#include "Framework/DX11.h"

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
	}

	ComPtr<ID3D11RenderTargetView> renderer = ((GraphicsEngine::Get()->UseEditor() == true) ? DX11::RenderRTV : DX11::BackBuffer);

	ID3D11RenderTargetView* buffers[2]
	{
		renderer.Get(),
		DX11::IDBuffer.Get()
	};

	DX11::Context->OMSetRenderTargets(2, &buffers[0], DX11::DepthBuffer.Get());
}

void GBuffer::SetAsResource(unsigned aStartSlot) const
{
	ID3D11ShaderResourceView* mySRVList[GBufferTexture::GB_COUNT];
	for (unsigned t = 0; t < mySRVs.size(); t++)
	{
		mySRVList[t] = mySRVs[t].Get();
	}

	DX11::Context->PSSetShaderResources(aStartSlot, GBufferTexture::GB_COUNT, &mySRVList[0]);
}

void GBuffer::ClearResource(unsigned aStartSlot) const
{

	ID3D11ShaderResourceView* srvList[GBufferTexture::GB_COUNT];
	for(int i = 0; i < GBufferTexture::GB_COUNT; i++)
	{
		srvList[i] = nullptr;
	}
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
		mySRVs[t]->Release();
	}

	for(unsigned t = 0; t < myRTVs.size(); t++)
	{
		myRTVs[t]->Release();
	}
}

bool GBuffer::CreateGBuffer()
{
	
		HRESULT result;

		RECT clientRect = DX11::GetClientSize();

		ComPtr<ID3D11Texture2D> bufferTex = (GraphicsEngine::Get()->UseEditor() == true) ? DX11::Texture2D[0] : DX11::BackBufferTex;
		


			
		D3D11_TEXTURE2D_DESC bufferDesc = { 0 };
		bufferDesc.Width = static_cast<UINT>(clientRect.right - clientRect.left);
		bufferDesc.Height = static_cast<UINT>(clientRect.bottom - clientRect.top);
		bufferDesc.ArraySize = 1;
		bufferDesc.SampleDesc.Count = 1;
		bufferDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		bufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

		result = DX11::Device->CreateTexture2D(&bufferDesc, nullptr, bufferTex.GetAddressOf());

		result = DX11::Device->CreateRenderTargetView(bufferTex.Get(), nullptr, myRTVs[GBuffer::GBufferTexture::GB_ALBEDO].GetAddressOf());
		if(FAILED(result))
			return false;
		result = DX11::Device->CreateShaderResourceView(bufferTex.Get(), nullptr, mySRVs[GBuffer::GBufferTexture::GB_ALBEDO].GetAddressOf());
		if(FAILED(result))
			return false;


		bufferTex = DX11::Texture2D[0];
		bufferDesc.Format = DXGI_FORMAT_R8G8B8A8_SNORM;

		result = DX11::Device->CreateTexture2D(&bufferDesc, nullptr, bufferTex.GetAddressOf());

		result = DX11::Device->CreateRenderTargetView(bufferTex.Get(), nullptr, myRTVs[GBuffer::GBufferTexture::GB_NORMAL].GetAddressOf());
		if(FAILED(result))
			return false;
		result = DX11::Device->CreateShaderResourceView(bufferTex.Get(), nullptr, mySRVs[GBuffer::GBufferTexture::GB_NORMAL].GetAddressOf());
		if(FAILED(result))
			return false;

		bufferTex = DX11::Texture2D[0];
		bufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

		result = DX11::Device->CreateTexture2D(&bufferDesc, nullptr, bufferTex.GetAddressOf());

		result = DX11::Device->CreateRenderTargetView(bufferTex.Get(), nullptr, myRTVs[GBuffer::GBufferTexture::GB_MATERIAL].GetAddressOf());
		if(FAILED(result))
			return false;
		result = DX11::Device->CreateShaderResourceView(bufferTex.Get(), nullptr, mySRVs[GBuffer::GBufferTexture::GB_MATERIAL].GetAddressOf());
		if(FAILED(result))
			return false;

		bufferTex = DX11::Texture2D[0];
		bufferDesc.Format = DXGI_FORMAT_R16G16B16A16_SNORM;

		result = DX11::Device->CreateTexture2D(&bufferDesc, nullptr, bufferTex.GetAddressOf());

		result = DX11::Device->CreateRenderTargetView(bufferTex.Get(), nullptr, myRTVs[GBuffer::GBufferTexture::GB_VERTEXNORMAL].GetAddressOf());
		if(FAILED(result))
			return false;
		result = DX11::Device->CreateShaderResourceView(bufferTex.Get(), nullptr, mySRVs[GBuffer::GBufferTexture::GB_VERTEXNORMAL].GetAddressOf());
		if(FAILED(result))
			return false;


		bufferTex = DX11::Texture2D[0];
		bufferDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		result = DX11::Device->CreateTexture2D(&bufferDesc, nullptr, bufferTex.GetAddressOf());

		result = DX11::Device->CreateRenderTargetView(bufferTex.Get(), nullptr, myRTVs[GBuffer::GBufferTexture::GB_POSITION].GetAddressOf());
		if(FAILED(result))
			return false;
		result = DX11::Device->CreateShaderResourceView(bufferTex.Get(), nullptr, mySRVs[GBuffer::GBufferTexture::GB_POSITION].GetAddressOf());
		if(FAILED(result))
			return false;


		bufferTex = DX11::Texture2D[0];
		bufferDesc.Format = DXGI_FORMAT_R8_UNORM;
		result = DX11::Device->CreateTexture2D(&bufferDesc, nullptr, bufferTex.GetAddressOf());

		result = DX11::Device->CreateRenderTargetView(bufferTex.Get(), nullptr, myRTVs[GBuffer::GBufferTexture::GB_AMBIENTOCCLUSION].GetAddressOf());
		if(FAILED(result))
			return false;
		result = DX11::Device->CreateShaderResourceView(bufferTex.Get(), nullptr, mySRVs[GBuffer::GBufferTexture::GB_AMBIENTOCCLUSION].GetAddressOf());
		if(FAILED(result))
			return false;



		return true;
}

bool DeferredRenderer::Init()
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

	bufferDescription.ByteWidth = sizeof(Vector4f);
	result = DX11::Device->CreateBuffer(&bufferDescription, nullptr, myMaterialBuffer.GetAddressOf());
	if(FAILED(result))
	{
		return false;
	}

	bufferDescription.ByteWidth = sizeof(Light::LightBufferData);
	result = DX11::Device->CreateBuffer(&bufferDescription, nullptr, myLightBuffer.GetAddressOf());
	if(FAILED(result))
	{
		return false;
	}


	myGBufferPS = TextureAssetHandler::GetPixelShader("Shaders\\GBDeferredPS.cso");
	myFullscreenVS = TextureAssetHandler::GetVertexShader("Shaders\\DeferredFullscreenVS.cso");
	myFullscreenPS = TextureAssetHandler::GetPixelShader("Shaders\\DeferredDefaultPS.cso");

	return true;
}

void DeferredRenderer::GenerateGBuffer(const std::vector<RenderBuffer>& aModelList, float aDeltaTime, float aTotalTime)
{
	HRESULT result = S_FALSE;
	D3D11_MAPPED_SUBRESOURCE bufferData;

	myFrameBufferData.View = Matrix4x4f::GetFastInverse(Renderer::GetViewMatrix());
	myFrameBufferData.CamTranslation = Renderer::GetViewMatrix().GetPosition();
	myFrameBufferData.Projection = Renderer::GetProjectionMatrix();
	myFrameBufferData.RenderMode = static_cast<int>(GraphicsEngine::Get()->GetRenderMode());

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


	for(const RenderBuffer& model : aModelList)
	{
		if(model.myModel == nullptr)
		{
			return;
		}


		myObjectBufferData.World = model.myTransform;
		ZeroMemory(&bufferData, sizeof(D3D11_MAPPED_SUBRESOURCE));

		myObjectBufferData.myObjectId = model.myId;
		myObjectBufferData.myHasBones = false;
		if(model.myModel->GetSkeleton()->GetRoot())
		{
			auto bones = model.myModel->GetBoneTransform();
			myObjectBufferData.myHasBones = true;
			memcpy_s(
				&myObjectBufferData.myBoneData[0], sizeof(Matrix4x4f) * 128,
				&bones[0], sizeof(Matrix4x4f) * 128
			);
		}

		result = DX11::Context->Map(myObjectBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferData);
		memcpy(bufferData.pData, &myObjectBufferData, sizeof(ObjectBufferData));
		DX11::Context->Unmap(myObjectBuffer.Get(), 0);

		




		if(model.myModel->GetMaterial())
		{
			model.myModel->GetMaterial()->SetAsResource(myMaterialBuffer);
		}
		else
		{

		}

		DX11::Context->PSSetShader(myGBufferPS.Get(), nullptr, 0);
		
		for(int index = 0; index < model.myModel->GetNumMeshes(); index++)
		{
			const Model::MeshData& meshData = model.myModel->GetMeshData(index);

			DX11::Context->IASetInputLayout(meshData.myInputLayout.Get());
			DX11::Context->VSSetShader(meshData.myVertexShader.Get(), nullptr, 0);
			DX11::Context->IASetVertexBuffers(0, 1, meshData.myVertexBuffer.GetAddressOf(), &meshData.myStride, &meshData.myOffset);
			DX11::Context->IASetIndexBuffer(meshData.myIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
			DX11::Context->IASetPrimitiveTopology(static_cast<D3D_PRIMITIVE_TOPOLOGY>(meshData.myPrimitiveTopology));
			
			DX11::Context->VSSetConstantBuffers(1, 1, myObjectBuffer.GetAddressOf());
			DX11::Context->PSSetConstantBuffers(1, 1, myObjectBuffer.GetAddressOf());

			DX11::Context->PSSetConstantBuffers(2, 1, myMaterialBuffer.GetAddressOf());

			DX11::Context->DrawIndexed(meshData.myNumberOfIndices, 0, 0);
		}
	}
}

void DeferredRenderer::Render(const std::shared_ptr<DirectionalLight>& aDirectionalLight,
	const std::shared_ptr<EnvironmentLight>& anEnvironmentLight, float aDetlaTime, float aTotalTime)
{
	HRESULT result = S_FALSE;
	D3D11_MAPPED_SUBRESOURCE bufferData;

	myFrameBufferData.View = Matrix4x4f::GetFastInverse(Renderer::GetViewMatrix());
	myFrameBufferData.Projection = Renderer::GetProjectionMatrix();
	myFrameBufferData.CamTranslation = Renderer::GetViewMatrix().GetPosition();

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

	if (aDirectionalLight)
	{
		aDirectionalLight->SetAsResource(myLightBuffer);
	}

	if (anEnvironmentLight)
	{
		anEnvironmentLight->SetAsResource(nullptr);
	}

	DX11::Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	DX11::Context->IASetInputLayout(nullptr);
	DX11::Context->IASetVertexBuffers(0, 0, nullptr, nullptr, nullptr);
	DX11::Context->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);
	DX11::Context->GSSetShader(nullptr, nullptr, 0);
	DX11::Context->VSSetShader(myFullscreenVS.Get(), nullptr, 0);
	DX11::Context->PSSetShader(myFullscreenPS.Get(), nullptr, 0);

	DX11::Context->Draw(3, 0);
}
