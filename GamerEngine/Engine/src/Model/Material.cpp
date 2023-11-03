#include "GraphicsEngine.pch.h"
#include <Model/Material.h>

#include <Framework/DX11.h>

Material::Material()
{
}

void Material::Init(std::wstring aName)
{
	Init(aName, { 1,1,1 });
}

void Material::Init(std::wstring aName, const Vector3f& anAlbedo)
{
	myName = aName;
	myMaterialData.Albedo = anAlbedo;
}

void Material::SetTexture(TextureType aTextureType, Ref<Texture> aTexture)
{
	switch (aTextureType)
	{
	case TextureType::Albedo:
		SetAlbedoTexture(aTexture);
		break;
	case TextureType::Normal:
		SetNormalTexture(aTexture);
		break;
	case TextureType::Material:
		SetMaterialTexture(aTexture);
		break;
	case TextureType::COUNT:
		break;
	default: ;
	}
}

void Material::SetAlbedoTexture(Ref<Texture> aTexture)
{
	TextureType type = TextureType::Albedo;

	aTexture->SetTextureType(type);
	myTextures[static_cast<int>(type)] = aTexture;
}

void Material::SetNormalTexture(Ref<Texture> aNormalTexture)
{
	TextureType type = TextureType::Normal;

	aNormalTexture->SetTextureType(type);
	myTextures[static_cast<int>(type)] = aNormalTexture;
}

void Material::SetMaterialTexture(Ref<Texture> aMaterialTexture)
{
	TextureType type = TextureType::Material;

	aMaterialTexture->SetTextureType(type);
	myTextures[static_cast<int>(type)] = aMaterialTexture;
}

void Material::SetAsResource(ComPtr<ID3D11Resource> aMaterialBuffer)
{
	//D3D11_MAPPED_SUBRESOURCE bufferData;
	//ZeroMemory(&bufferData, sizeof(D3D11_MAPPED_SUBRESOURCE));

	//const HRESULT result = 
	//
	// ->Map(
	//	aMaterialBuffer.Get(),
	//	0,
	//	D3D11_MAP_WRITE_DISCARD,
	//	0,
	//	&bufferData
	//	);

	//if (FAILED(result))
	//{
	//	// ?? what happen?????????
	//}

	//memcpy(bufferData.pData, &myMaterialData, sizeof(MaterialData));

	//DX11::Get().GetContext()->Unmap(aMaterialBuffer.Get(), 0);

	if (myTextures[static_cast<int>(TextureType::Albedo)])
	{
		myTextures[static_cast<int>(TextureType::Albedo)]->SetAsResource(0);
	}
	if (myTextures[static_cast<int>(TextureType::Normal)])
	{
		myTextures[static_cast<int>(TextureType::Normal)]->SetAsResource(1);
	}
	if(myTextures[static_cast<int>(TextureType::Material)])
	{
		myTextures[static_cast<int>(TextureType::Material)]->SetAsResource(2);
	}
}
