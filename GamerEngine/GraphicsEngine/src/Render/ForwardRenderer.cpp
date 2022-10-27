#include "GraphicsEngine.pch.h"
#include "ForwardRenderer.h"
#include <Framework/DX11.h>
#include <Math/Matrix4x4.hpp>
#include <Math/MathTypes.hpp>

#include "GraphicsEngine.h"
#include "Renderer.h"
#include "Light/DirectionalLight.h"
#include "Light/EnvironmentLight.h"
#include "Sort/MergeSort.hpp"
#include "Sort/QuickSort.hpp"
#include <Particles/particleemitter.h>

void ForwardRenderer::SetDepthStencilState(DepthStencilState aDepthStencilState)
{
	DX11::Context->OMSetDepthStencilState(myDepthStencilStates[(int)aDepthStencilState].Get(), 0xffffffff);
}

void ForwardRenderer::SetBlendState(BlendState aBlendState)
{
	DX11::Context->OMSetBlendState(myBlendStates[(int)aBlendState].Get(), nullptr, 0xffffffff);
}

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

	bufferDescription.ByteWidth = sizeof(Light::LightBufferData);
	result = DX11::Device->CreateBuffer(&bufferDescription, nullptr, myLightBuffer.GetAddressOf());
	if(FAILED(result))
	{
		return false;
	}

	BuildDepth();

    return true;
}

void ForwardRenderer::Render(const std::vector<RenderBuffer>& aModelList, const std::shared_ptr<DirectionalLight>& aDirectionalLight, const std::shared_ptr<EnvironmentLight>& anEnvironmentLight)
{
	HRESULT result = S_FALSE;
	D3D11_MAPPED_SUBRESOURCE bufferData;

	ID3D11RenderTargetView* buffers[2];
	buffers[0] = DX11::RenderRTV.Get();
	buffers[1] = DX11::IDBuffer.Get();
	DX11::Context->OMSetRenderTargets(2, buffers, DX11::DepthBuffer.Get());


	myFrameBufferData.View = Matrix4x4f::GetFastInverse(Renderer::GetViewMatrix());
	myFrameBufferData.CamTranslation = Renderer::GetViewMatrix().GetPosition();
	myFrameBufferData.Projection = Renderer::GetProjectionMatrix();
	myFrameBufferData.RenderMode = static_cast<int>(GraphicsEngine::Get()->GetRenderMode());
	
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

	SetBlendState(BlendState::AlphaBlend);
	SetDepthStencilState(DepthStencilState::DSSReadWrite);

	if(aDirectionalLight)
	{
		aDirectionalLight->SetAsResource(myLightBuffer);
	}

	if(anEnvironmentLight)
	{
		anEnvironmentLight->SetAsResource(nullptr);
	}



	for (const RenderBuffer& model : aModelList)
	{
		if (model.myModel == nullptr)
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
		if(FAILED(result))
		{

		}

		memcpy(bufferData.pData, &myObjectBufferData, sizeof(ObjectBufferData));
		DX11::Context->Unmap(myObjectBuffer.Get(), 0);

		
		for(int index = 0; index < static_cast<int>(model.myModel->GetNumMeshes()); index++)
		{
			if(model.myModel->GetMaterial())
			{
				model.myModel->GetMaterial()->SetAsResource(myMaterialBuffer);
				
			}
			else
			{

			}

			

			for(int index = 0; index < model.myModel->GetNumMeshes(); index++)
			{
				const Model::MeshData& meshData = model.myModel->GetMeshData(index);

				DX11::Context->VSSetConstantBuffers(1, 1, myObjectBuffer.GetAddressOf());
				DX11::Context->PSSetConstantBuffers(1, 1, myObjectBuffer.GetAddressOf());
				DX11::Context->PSSetConstantBuffers(2, 1, myMaterialBuffer.GetAddressOf());

				//DX11::Context->IASetVertexBuffers(0, 1, meshData.myVertexBuffer.GetAddressOf(), &meshData.myStride, &meshData.myOffset);
				//DX11::Context->IASetIndexBuffer(meshData.myIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

				//DX11::Context->IASetPrimitiveTopology(static_cast<D3D_PRIMITIVE_TOPOLOGY>(meshData.myPrimitiveTopology));
				//DX11::Context->IASetInputLayout(meshData.myInputLayout.Get());

				//DX11::Context->VSSetShader(meshData.myVertexShader.Get(), nullptr, 0);
				//DX11::Context->GSSetShader(nullptr, nullptr, 0);
				//DX11::Context->PSSetShader(meshData.myPixelShader.Get(), nullptr, 0);

				//DX11::Context->DrawIndexed(meshData.myNumberOfIndices, 0, 0);

				DX11::Context->IASetInputLayout(meshData.myInputLayout.Get());
				DX11::Context->VSSetShader(meshData.myVertexShader.Get(), nullptr, 0);
				DX11::Context->GSSetShader(meshData.myGeometryShader.Get(), nullptr, 0);
				DX11::Context->PSSetShader(meshData.myPixelShader.Get(), nullptr, 0);

				DX11::Context->IASetVertexBuffers(0, 1, meshData.myVertexBuffer.GetAddressOf(), &meshData.myStride, &meshData.myOffset);
				DX11::Context->IASetIndexBuffer(meshData.myIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
				DX11::Context->IASetPrimitiveTopology((D3D_PRIMITIVE_TOPOLOGY)meshData.myPrimitiveTopology);
				DX11::Context->DrawIndexed(meshData.myNumberOfIndices, 0, 0);
			}
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
	SetDepthStencilState(DepthStencilState::DSSReadOnly);


	DX11::Context->VSSetConstantBuffers(0, 1, myFrameBuffer.GetAddressOf());
	DX11::Context->GSSetConstantBuffers(0, 1, myFrameBuffer.GetAddressOf());
	DX11::Context->PSSetConstantBuffers(0, 1, myFrameBuffer.GetAddressOf());
	DX11::Context->VSSetConstantBuffers(1, 1, myObjectBuffer.GetAddressOf());
	DX11::Context->PSSetConstantBuffers(1, 1, myObjectBuffer.GetAddressOf());
	CommonUtilities::MergeSort(aSpriteList);

	ID3D11RenderTargetView* buffers[2];
	buffers[0] = DX11::RenderRTV.Get();
	buffers[1] = DX11::IDBuffer.Get();
	DX11::Context->OMSetRenderTargets(2, buffers, DX11::DepthBuffer.Get());

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

		SetDepthStencilState(DepthStencilState::DSSReadOnly);

		rc.mySprite->SetAsResource();
		rc.mySprite->Draw();
	}
}

void ForwardRenderer::BuildDepth()
{
	HRESULT result;

	D3D11_SAMPLER_DESC samplerDesc;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 1.f;
	samplerDesc.BorderColor[1] = 1.f;
	samplerDesc.BorderColor[2] = 1.f;
	samplerDesc.BorderColor[3] = 1.f;
	samplerDesc.MinLOD = -D3D11_FLOAT32_MAX;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	result = DX11::Device->CreateSamplerState(&samplerDesc, mySamplerStates[(int)SamplerState::SSTrilinearClamp].GetAddressOf());
	DX11::Context->PSSetSamplers(0, 1, mySamplerStates[(int)SamplerState::SSTrilinearClamp].GetAddressOf());

	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	result = DX11::Device->CreateSamplerState(&samplerDesc, mySamplerStates[(int)SamplerState::SSPointClamp].GetAddressOf());
	DX11::Context->PSSetSamplers(1, 1, mySamplerStates[(int)SamplerState::SSPointClamp].GetAddressOf());

	myBlendStates[(int)BlendState::AdditiveBlend] = nullptr;

	D3D11_BLEND_DESC blendDesc = {};
	blendDesc.RenderTarget[0].BlendEnable = true;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_MAX;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	result = DX11::Device->CreateBlendState(&blendDesc, &myBlendStates[(int)BlendState::AlphaBlend]);


	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	result = DX11::Device->CreateBlendState(&blendDesc, &myBlendStates[(int)BlendState::AdditiveBlend]);


	myDepthStencilStates[(int)DepthStencilState::DSSReadWrite] = nullptr;

	D3D11_DEPTH_STENCIL_DESC depthDesc = {};
	depthDesc.DepthEnable = true;
	depthDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	depthDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthDesc.StencilEnable = false;
	result = DX11::Device->CreateDepthStencilState(&depthDesc, &myDepthStencilStates[(int)DepthStencilState::DSSReadOnly]);
	

	depthDesc.DepthEnable = false;
	depthDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	depthDesc.DepthFunc = D3D11_COMPARISON_NEVER;
	result = DX11::Device->CreateDepthStencilState(&depthDesc, &myDepthStencilStates[(int)DepthStencilState::DSSIgnore]);


	D3D11_DEPTH_STENCILOP_DESC depthOPDesc = {};
	depthDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthDesc.DepthFunc = D3D11_COMPARISON_ALWAYS;
	depthOPDesc.StencilPassOp = D3D11_STENCIL_OP_INCR;
	//depthDesc.StencilEnable = true;
	depthDesc.FrontFace = depthOPDesc;
	result = DX11::Device->CreateDepthStencilState(&depthDesc, &myDepthStencilStates[(int)DepthStencilState::DSSOverlay]);
	
}

void ForwardRenderer::ClearBuffer()
{
}

void ForwardRenderer::Release()
{
	myFrameBuffer->Release();
	myObjectBuffer->Release();
	myLightBuffer->Release();
	myMaterialBuffer->Release();

	for (int i = 0; i < myDepthStencilStates.size(); i++)
	{
		myDepthStencilStates[i] = nullptr;
	}
}
