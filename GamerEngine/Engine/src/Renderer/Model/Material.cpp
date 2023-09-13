#include "GraphicsEngine.pch.h"
#include <Renderer/Model/Material.h>

#include <Renderer/Framework/DX11.h>

Material::Material()
{
}

void Material::Init(std::wstring aName, const Vector3f& anAlbedo)
{
	myName = aName;
	myMaterialData.Albedo = anAlbedo;
}

void Material::SetAlbedoTexture(std::shared_ptr<Texture> aTexture)
{
	myTextures[MaterialTextureChannel::Albedo] = aTexture;
}

void Material::SetNormalTexture(std::shared_ptr<Texture> aNormalTexture)
{
	myTextures[MaterialTextureChannel::Normal] = aNormalTexture;
}

void Material::SetMaterialTexture(std::shared_ptr<Texture> aMaterialTexture)
{
	myTextures[MaterialTextureChannel::Material] = aMaterialTexture;
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

	//DX11::GetContext()->Unmap(aMaterialBuffer.Get(), 0);

	if (myTextures[MaterialTextureChannel::Albedo])
	{
		myTextures[MaterialTextureChannel::Albedo]->SetAsResource(0);
	}
	if (myTextures[MaterialTextureChannel::Normal])
	{
		myTextures[MaterialTextureChannel::Normal]->SetAsResource(1);
	}
	if(myTextures[MaterialTextureChannel::Material])
	{
		myTextures[MaterialTextureChannel::Material]->SetAsResource(2);
	}
}
