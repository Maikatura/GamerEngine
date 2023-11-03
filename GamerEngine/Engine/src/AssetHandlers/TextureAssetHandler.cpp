#include "GraphicsEngine.pch.h"

#include <iostream>
#include <AssetHandlers/TextureAssetHandler.h>
#include <Model/Texture.h>
#include <AssetHandlers/DDSTextureLoader11.h>
#include <Framework/DX11.h>
#include <filesystem>
#include <fstream>

#include "DDSTextureLoader.h"
#include "Render/DepthStencil.h"
#include "Scene/SceneManager.h"
#include <WICTextureLoader.h>

#include "Utilites/StringCast.h"
#include "Utilites/COMInitializer.h"


void TextureAssetHandler::Clear()
{
	myRegistry.clear();
}

Ref<Texture> TextureAssetHandler::GetTexture(const std::wstring& aName)
{
	
	std::wstring texName = Helpers::ToLowerCase(aName);

	auto model = myRegistry.find(texName);

	if (model == myRegistry.end())
	{
		LoadTexture(texName);
	}

	model = myRegistry.find(texName);

	if(model == myRegistry.end())
	{
		model = myRegistry.find(L"resources\\textures\\T_Default_C.dds");

		if (model == myRegistry.end())
		{
			LoadTexture(L"resources\\textures\\T_Default_C.dds");
		}

		if (myRegistry.empty())
		{
			return MakeRef<Texture>();
		}

		return myRegistry.find(L"resources\\textures\\T_Default_C.dds")->second;
	}

	

	return model->second;
}

bool TextureAssetHandler::LoadTexture(const std::wstring& aFileName)
{
	if (SceneManager::Get().IsHeadless())
	{
		return false;
	}

	if (const auto It = myRegistry.find(aFileName); It == myRegistry.end())
	{

		std::filesystem::path filename = aFileName;

		Ref<Texture> result = MakeRef<Texture>();
		HRESULT createResult = S_FALSE;
		if (filename.extension() == ".dds")
		{
			createResult = DirectX::CreateDDSTextureFromFile(DX11::Get().Get().GetDevice(), aFileName.c_str(),
				result->myTexture.GetAddressOf(),
				result->mySRV.GetAddressOf()
			);
		}
		else
		{
			createResult = DirectX::CreateWICTextureFromFile(DX11::Get().Get().GetDevice(), aFileName.c_str(),
				result->myTexture.GetAddressOf(),
				result->mySRV.GetAddressOf()
			);
		}

		


		std::filesystem::path path = aFileName;
		result->SetName(path.filename());
		result->SetPath(path.wstring());

		if (SUCCEEDED(createResult))
		{
			
			myRegistry.insert({ path.wstring(), result });
			return true;
		}

	}

	return false;
}

void TextureAssetHandler::UnloadUnusedTextures()
{
	
}

Ref<DepthStencil> TextureAssetHandler::CreateDepthStencil(
	const std::wstring& aName, size_t aWidth, size_t aHeight)
{
	HRESULT result = S_FALSE;

	std::unique_ptr<DepthStencil> output = std::make_unique<DepthStencil>();
	output->myName = aName;

	D3D11_TEXTURE2D_DESC desc = { 0 };
	desc.Width = static_cast<unsigned int>(aWidth);
	desc.Height = static_cast<unsigned int>(aHeight);
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT::DXGI_FORMAT_R32_TYPELESS;
	desc.SampleDesc.Quality = 0;
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;
	result = DX11::Get().GetDevice()->CreateTexture2D(&desc, nullptr, reinterpret_cast<ID3D11Texture2D**>(output->myTexture.GetAddressOf()));
	assert(SUCCEEDED(result));


	D3D11_SHADER_RESOURCE_VIEW_DESC resourceDesc = {};
	ZeroMemory(&resourceDesc, sizeof(resourceDesc));
	resourceDesc.Format = DXGI_FORMAT::DXGI_FORMAT_R32_FLOAT;
	resourceDesc.ViewDimension = D3D11_SRV_DIMENSION::D3D11_SRV_DIMENSION_TEXTURE2D;
	resourceDesc.Texture2D.MipLevels = desc.MipLevels;
	result = DX11::Get().GetDevice()->CreateShaderResourceView(output->myTexture.Get(), &resourceDesc, output->mySRV.ReleaseAndGetAddressOf());
	assert(SUCCEEDED(result));


	D3D11_DEPTH_STENCIL_VIEW_DESC depthDesc = {};
	depthDesc.Format = DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT;
	depthDesc.ViewDimension = D3D11_DSV_DIMENSION::D3D11_DSV_DIMENSION_TEXTURE2D;
	result = DX11::Get().GetDevice()->CreateDepthStencilView(output->myTexture.Get(), &depthDesc, output->myDSV.ReleaseAndGetAddressOf());
	assert(SUCCEEDED(result));

	output->myViewport = D3D11_VIEWPORT({ 0.0f, 0.0f, static_cast<float>(aWidth), static_cast<float>(aHeight), 0.0f, 1.0f });

	return output;
}

ID3D11PixelShader* TextureAssetHandler::GetPixelShader(const std::string& aPath)
{
	std::ifstream psFile;
	psFile.open(aPath.c_str(), std::ios::binary);
	std::string psData = { std::istreambuf_iterator<char>(psFile), std::istreambuf_iterator<char>() };
	ID3D11PixelShader* pixelShader;
	DX11::Get().GetDevice()->CreatePixelShader(psData.data(), psData.size(), nullptr, &pixelShader);
	
	psFile.close();

	return pixelShader;
}

ID3D11VertexShader* TextureAssetHandler::GetVertexShader(const std::string& aPath)
{
	std::ifstream vsFile;
	vsFile.open(aPath, std::ios::binary);
	std::string vsData = { std::istreambuf_iterator<char>(vsFile), std::istreambuf_iterator<char>() };
	ID3D11VertexShader* vertexShader;
	HRESULT result = DX11::Get().GetDevice()->CreateVertexShader(vsData.data(), vsData.size(), nullptr, &vertexShader);
	if(FAILED(result))
	{
		// WTF
	}
	vsFile.close();

	return vertexShader;
}

std::vector<Ref<Texture>> TextureAssetHandler::GetTextureList()
{
	std::vector<Ref<Texture>> textureList;

	for (auto [name, texture] : myRegistry)
	{
		textureList.push_back(texture);
	}

	return textureList;
}