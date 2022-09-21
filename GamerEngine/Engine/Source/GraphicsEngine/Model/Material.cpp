#include "GraphicsEngine.pch.h"
#include <Model/Material.h>

#include <Framework/DX11.h>

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

void Material::SetAsResource(ComPtr<ID3D11Resource> aMaterialBuffer)
{
	D3D11_MAPPED_SUBRESOURCE bufferData;
	ZeroMemory(&bufferData, sizeof(D3D11_MAPPED_SUBRESOURCE));

	const HRESULT result = DX11::Context->Map(
		aMaterialBuffer.Get(),
		0,
		D3D11_MAP_WRITE_DISCARD,
		0,
		&bufferData
		);

	if (FAILED(result))
	{
		// ?? WTF what happend?????????
	}

	memcpy_s(bufferData.pData, sizeof(MaterialData), &myMaterialData, sizeof(MaterialData));

	DX11::Context->Unmap(aMaterialBuffer.Get(), 0);

	if (myTextures[MaterialTextureChannel::Albedo])
	{
		myTextures[MaterialTextureChannel::Albedo]->SetAsResource(0);
	}
}
