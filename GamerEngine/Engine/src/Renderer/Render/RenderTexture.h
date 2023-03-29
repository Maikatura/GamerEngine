#pragma once
#include <Math/MathTypes.hpp>
#include "Renderer/Framework/DX11.h"
#include "Renderer/Model/Texture.h"
#include "Renderer/Render/DepthStencil.h"

struct RenderTexture : public Texture
{
	Vector2f mySize;

	Microsoft::WRL::ComPtr<ID3D11Texture2D> myTexture2D;
	std::shared_ptr<DepthStencil> myDepthStencil;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> myRTV;


	void SetAsTarget()
	{
		D3D11_VIEWPORT viewport = {};
		viewport.TopLeftX = 0.f;
		viewport.TopLeftY = 0.f;
		viewport.Width = mySize.x;
		viewport.Height = mySize.y;
		viewport.MinDepth = 0.f;
		viewport.MaxDepth = 1.f;
		DX11::Context->RSSetViewports(1, &viewport);
		DX11::Context->OMSetRenderTargets(1, myRTV.GetAddressOf(), myDepthStencil->myDSV.Get());
	}

	void Clear()
	{
		Vector4f clCol = { 0, 0, 0, 0 };
		DX11::Context->ClearRenderTargetView(myRTV.Get(), &clCol.x);
		DX11::Context->ClearDepthStencilView(myDepthStencil->myDSV.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
	}

	static std::shared_ptr<RenderTexture> Create(int aWidth, int aHeight, DXGI_FORMAT aFromat)
	{
		std::unique_ptr<RenderTexture> output = std::make_unique<RenderTexture>();


		DXGI_SAMPLE_DESC sampleDesc{};
		sampleDesc.Quality = 0;
		sampleDesc.Count = 1;

		D3D11_TEXTURE2D_DESC desc{};
		desc.BindFlags = D3D11_BIND_RENDER_TARGET;
		desc.Width = aWidth;
		desc.Height = aHeight;
		/*desc.ArraySize = myDescription.IsTextureCube * 5 + 1;
		desc.MipLevels = 1;
		desc.SampleDesc = sampleDesc;
		desc.Usage = DXUtil::AsD3D11Usage(BufferUsage::Default);
		desc.Format = DXUtil::AsDXGIFormat(myDescription.Format);
		desc.CPUAccessFlags = DXUtil::AsD3D11CpuAccessFlags(CPUAccessFlags::None);*/

		//if(myDescription.CreateUAV)
		//{
		//	desc.BindFlags = D3D11_BIND_UNORDERED_ACCESS;
		//}

		//if(myDescription.IsTextureCube)
		//{
		//	desc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;
		//}

		//if(!CommonUtilities::BitUtil::HasFlag(myDescription.Flags, RenderTargetFlags::NoResource))
		//{
		//	desc.BindFlags |= D3D11_BIND_SHADER_RESOURCE;
		//}
		//THROW_IF_FAILED(DX11::GetDevice().Get()->CreateTexture2D(&desc, nullptr, myResource.ReleaseAndGetAddressOf()));

		//if(myDescription.CreateUAV)
		//{
		//	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
		//	ZeroMemory(&uavDesc, sizeof(uavDesc));
		//	uavDesc.Format = desc.Format;// DXGI_FORMAT_UNKNOWN;//DXGI_FORMAT_R16G16B16A16_FLOAT;
		//	uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
		//	uavDesc.Texture2D.MipSlice = 0;


		//	THROW_IF_FAILED(DX11::GetDevice().Get()->CreateUnorderedAccessView(myResource.Get(), &uavDesc, myUnorderedAccessView.ReleaseAndGetAddressOf()));
		//}
		//else
		//{
		//	D3D11_RENDER_TARGET_VIEW_DESC renderDesc;
		//	renderDesc.Format = desc.Format;
		//	if(myDescription.IsTextureCube)
		//	{
		//		renderDesc.Texture2DArray.ArraySize = 6;
		//		renderDesc.Texture2DArray.FirstArraySlice = 0;
		//		renderDesc.Texture2DArray.MipSlice = 0;
		//		renderDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
		//	}
		//	else
		//	{
		//		renderDesc.Texture2D.MipSlice = 0;
		//		renderDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		//	}
		//	THROW_IF_FAILED(DX11::GetDevice().Get()->CreateRenderTargetView(myResource.Get(), &renderDesc, myRenderTargetView.ReleaseAndGetAddressOf()));
		//}

		//D3D11_SHADER_RESOURCE_VIEW_DESC resourceViewDesc{};
		//resourceViewDesc.Format = desc.Format;
		//if(myDescription.IsTextureCube)
		//{
		//	resourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
		//}
		//else
		//{
		//	resourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		//}
		//resourceViewDesc.Texture2D.MipLevels = desc.MipLevels;
		//THROW_IF_FAILED(DX11::GetDevice().Get()->CreateShaderResourceView(myResource.Get(), &resourceViewDesc, myShaderResourceView.ReleaseAndGetAddressOf()));

		//if(!CommonUtilities::BitUtil::HasFlag(myDescription.Flags, RenderTargetFlags::UseDefaultDepthBuffer))
		//{
		//	if(myDepthStencil)
		//	{
		//		myDepthStencil->Invalidate(aWidth, aHeight);
		//	}
		//	else if(myDescription.DepthStencilDescription.Format != DepthTextureFormat::Unknown)
		//	{
		//		myDepthStencil = DepthStencil::Create(myDescription.DepthStencilDescription);
		//	}
		//}

		//myDescription.Width = aWidth;
		//myDescription.Height = aHeight;

		output->myDepthStencil = TextureAssetHandler::CreateDepthStencil(L"RenderTarget", aWidth, aHeight);

		return output;
	}
};
