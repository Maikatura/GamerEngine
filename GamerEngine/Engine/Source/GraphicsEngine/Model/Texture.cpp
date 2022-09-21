#include "GraphicsEngine.pch.h"
#include <Model/Texture.h>
#include <Framework/DX11.h>


Texture::~Texture()
{
	myTexture.Reset();
	mySRV.Reset();
}

void Texture::SetAsResource(unsigned aSlot)
{
	DX11::Context->PSSetShaderResources(aSlot, 1, mySRV.GetAddressOf());
}
