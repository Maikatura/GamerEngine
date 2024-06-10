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

	~DepthStencil()
	{
		myTexture.Reset();
		mySRV.Reset();
		myDSV.Reset();
	}

	void ClearBuffer()
	{
		//DX11::Get().GetContext()->ClearDepthStencilView(myDSV.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}
};
