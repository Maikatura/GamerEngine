#include "GraphicsEngine.pch.h"

#include <iostream>
#include <AssetHandlers/TextureAssetHandler.h>
#include <Model/Texture.h>
#include <AssetHandlers/DDSTextureLoader11.h>
#include <Framework/DX11.h>

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
		LoadTexture(L"Assets\\Models\\T_Default_C.dds");
		return myRegistry.find(L"Assets\\Models\\T_Default_C.dds")->second;
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
