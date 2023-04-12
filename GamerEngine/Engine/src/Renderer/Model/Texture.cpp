#include "GraphicsEngine.pch.h"
#include <Renderer/Model/Texture.h>
#include <Renderer/Framework/DX11.h>


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
	DX11::Context->VSSetShaderResources(aSlot, 1, mySRV.GetAddressOf());
	DX11::Context->PSSetShaderResources(aSlot, 1, mySRV.GetAddressOf());
}

void Texture::SetPath(const std::wstring& aPath)
{
	myPath = aPath;
}