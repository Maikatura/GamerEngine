#pragma once
#include <unordered_map>

#include "RendererBase.h"
#include "Framework/DX11.h"
#include "Model/Texture.h"

class PostProcessRenderer : public RendererBase
{
public:
	enum PostProcessPass
	{
		PP_COPY,
		PP_LUMINANCE,
		PP_GAUSSIAN,
		PP_BLOOM,
		PP_SSAO,
		PP_TONEMAP,
		PP_COUNT
	};

	struct RenderTexture : public Texture
	{
		Vector2f mySize;

		ID3D11Texture2D* myTexture2D;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> myDSV;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> myRTV;

		void SetAsTarget()
		{
			D3D11_VIEWPORT viewport = {};
			viewport.TopLeftX = 0.f;
			viewport.TopLeftY = 0.f;
			viewport.Width = mySize.x;
			viewport.Height = mySize.y;
			viewport.MinDepth = 0.f;
			viewport.MaxDepth = 1.f;
			DX11::Context->RSSetViewports(1, &viewport);
			DX11::Context->OMSetRenderTargets(1, myRTV.GetAddressOf(), myDSV.Get());
		}

		void Clear()
		{
			Vector4f clCol = { 0, 0, 0, 0 };
			DX11::Context->ClearRenderTargetView(myRTV.Get(), &clCol.x);
			DX11::Context->ClearDepthStencilView(myDSV.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
		}
	};

private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> myFullscreenVS;
	std::array<Microsoft::WRL::ComPtr<ID3D11PixelShader>, PostProcessPass::PP_COUNT> myPassShaders;
	std::unordered_map<std::string, RenderTexture> myRenderTextures;

	RenderTexture mySSAOTexture;
	RenderTexture myFullSize;
	RenderTexture myHalfSize;
	RenderTexture myQuarterSize;
	RenderTexture myBlur;

	std::shared_ptr<Texture> myNoiseTexture;
	bool myHasInited = false;

public:

	bool Initialize();
	void ReInitialize();

	RenderTexture CreateRenderTexture(std::string aName, UINT aWidth, UINT aHeight, DXGI_FORMAT aFormat = DXGI_FORMAT_R8G8B8A8_UNORM);

	void Render(PostProcessPass aPass);
	void RenderPass(PostProcessPass aPass);

	void Release();
	void ClearTargets();
};
