#include "GraphicsEngine.pch.h"

#include <iostream>
#include <AssetHandlers/TextureAssetHandler.h>
#include <Model/Texture.h>
#include <AssetHandlers/DDSTextureLoader11.h>
#include <Framework/DX11.h>
#include <filesystem>
#include <fstream>

void TextureAssetHandler::Clear()
{
	myRegistry.clear();
}

std::shared_ptr<Texture> TextureAssetHandler::GetTexture(const std::wstring& aName)
{
	auto model = myRegistry.find(aName);

	if (model == myRegistry.end())
	{
		LoadTexture(aName);
	}

	model = myRegistry.find(aName);

	if(model == myRegistry.end())
	{
		LoadTexture(L"Assets\\Textures\\T_Default_C.dds");
		return myRegistry.find(L"Assets\\Textures\\T_Default_C.dds")->second;
	}


	return model->second;
}

bool TextureAssetHandler::LoadTexture(const std::wstring& aFileName)
{
	if (const auto It = myRegistry.find(aFileName); It == myRegistry.end())
	{



		Texture result;
		const HRESULT createResult = DirectX::CreateDDSTextureFromFile(DX11::Device.Get(), aFileName.c_str(),
			result.myTexture.GetAddressOf(),
			result.mySRV.GetAddressOf()
		);


		std::filesystem::path path = aFileName;
		result.SetName(path.filename());

		if (SUCCEEDED(createResult))
		{
			myRegistry.insert({ aFileName, std::make_shared<Texture>(result) });
		}
	}

	return false;
}

void TextureAssetHandler::UnloadUnusedTextures()
{
	
}

ID3D11PixelShader* TextureAssetHandler::GetPixelShader(const std::string& aPath)
{
	std::ifstream psFile;
	psFile.open(aPath.c_str(), std::ios::binary);
	std::string psData = { std::istreambuf_iterator<char>(psFile), std::istreambuf_iterator<char>() };
	ID3D11PixelShader* pixelShader;
	DX11::Device->CreatePixelShader(psData.data(), psData.size(), nullptr, &pixelShader);
	
	psFile.close();

	return pixelShader;
}

ID3D11VertexShader* TextureAssetHandler::GetVertexShader(const std::string& aPath)
{
	std::ifstream vsFile;
	vsFile.open(aPath, std::ios::binary);
	std::string vsData = { std::istreambuf_iterator<char>(vsFile), std::istreambuf_iterator<char>() };
	ID3D11VertexShader* vertexShader;
	HRESULT result = DX11::Device->CreateVertexShader(vsData.data(), vsData.size(), nullptr, &vertexShader);
	if(FAILED(result))
	{
		// WTF
	}
	vsFile.close();

	return vertexShader;
}
