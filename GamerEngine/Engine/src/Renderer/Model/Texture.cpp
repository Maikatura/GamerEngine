#include "GraphicsEngine.pch.h"
#include <Renderer/Model/Texture.h>
#include <Renderer/Framework/DX11.h>


TextureType Texture::GetTextureType()
{
	return myTextureType;
}

void Texture::SetTextureType(TextureType aTextureType)
{
	myTextureType = aTextureType;
}

Texture::Texture()
{
}

Texture::~Texture()
{
	myTexture.Reset();
	mySRV.Reset();
}

void Texture::SetName(const std::wstring& aName)
{
	myName = aName;
}

void Texture::SetAsResource(unsigned aSlot)
{
	DX11::GetContext()->PSSetShaderResources(aSlot, 1, mySRV.GetAddressOf());
}

void Texture::ClearResource(unsigned aSlot)
{
	DX11::GetContext()->PSSetShaderResources(aSlot, 1, nullptr);
}

void Texture::SetPath(const std::wstring& aPath)
{
	myPath = aPath;
}
