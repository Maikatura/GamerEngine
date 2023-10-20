#pragma once
#include <unordered_map>

#include "DepthStencil.h"
#include "RendererBase.h"
#include "RenderTexture.h"
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

	

private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> myFullscreenVS;
	std::array<Microsoft::WRL::ComPtr<ID3D11PixelShader>, PostProcessPass::PP_COUNT> myPassShaders;

	std::shared_ptr<RenderTexture> mySSAOTexture;
	std::shared_ptr<RenderTexture> myFullSize;
	std::shared_ptr<RenderTexture> myHalfSize;
	std::shared_ptr<RenderTexture> myQuarterSize;
	std::shared_ptr<RenderTexture> myBlur;

	std::shared_ptr<Texture> myNoiseTexture;
	bool myHasInited = false;

public:

	bool Initialize();
	void ReInitialize();

	std::shared_ptr<RenderTexture> CreateRenderTexture(const std::string& aName, float aWidth, float aHeight, DXGI_FORMAT aFormat = DXGI_FORMAT_R8G8B8A8_UNORM);

	void Render(PostProcessPass aPass, Matrix4x4f aView, Matrix4x4f aProjection);


	void RenderTextureOnSlot(int aSlot, int aResourceSlot, PostProcessPass aPass, std::shared_ptr<RenderTexture> aRenderTexture);
	void RenderPass(PostProcessPass aPass);

	void Release();
	void ClearTargets();
};
