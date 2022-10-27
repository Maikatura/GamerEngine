#include "GraphicsEngine.pch.h"
#include <Light/DirectionalLight.h>

#include "Framework/DX11.h"

void DirectionalLight::SetAsResource(Microsoft::WRL::ComPtr<ID3D11Buffer> aLightBuffer)
{
	D3D11_MAPPED_SUBRESOURCE bufferData;
	ZeroMemory(&bufferData, sizeof(D3D11_MAPPED_SUBRESOURCE));

	const HRESULT result = DX11::Context->Map(
		aLightBuffer.Get(),
		0,
		D3D11_MAP_WRITE_DISCARD,
		0,
		&bufferData);

	if (FAILED(result))
	{
		// NOOOOOOOOOOOOOO :(
	}

	memcpy(bufferData.pData, &myLightBufferData, sizeof(LightBufferData));

	DX11::Context->Unmap(aLightBuffer.Get(), 0);

	DX11::Context->PSSetConstantBuffers(3, 1, aLightBuffer.GetAddressOf());
}
