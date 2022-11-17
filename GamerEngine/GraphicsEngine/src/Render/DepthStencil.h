#pragma once
#include <wrl/client.h>
#include <d3d11.h>

class DepthStencil
{
public:
	Microsoft::WRL::ComPtr<ID3D11Texture2D> myTexture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mySRV;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> myDSV;
	D3D11_VIEWPORT myViewport;
	std::wstring myName;
};
