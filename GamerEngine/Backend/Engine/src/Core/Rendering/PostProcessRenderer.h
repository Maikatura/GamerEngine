#pragma once
#include <unordered_map>

#include "DepthStencil.h"
#include "RendererBase.h"
#include "RenderTexture.h"
#include "Core/Framework/DX11.h"
#include "Math/Frustum.h"
#include "Core/Model/Texture.h"

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

	Ref<RenderTexture> mySSAOTexture;
	Ref<RenderTexture> myFullSize;
	Ref<RenderTexture> myHalfSize;
	Ref<RenderTexture> myQuarterSize;
	Ref<RenderTexture> myBlur;

	Ref<Texture> myNoiseTexture;
	bool myHasInited = false;

	CommonUtilities::Frustum myFrustum;
	
public:

	static PostProcessRenderer& Get()
	{
		static PostProcessRenderer instance;
		return instance;
	}

	bool Initialize();
	void ReInitialize();

	Ref<RenderTexture> CreateRenderTexture(const std::string& aName, float aWidth, float aHeight, DXGI_FORMAT aFormat = DXGI_FORMAT_R8G8B8A8_UNORM);

	void Render(PostProcessPass aPass, Matrix4x4f aView, Matrix4x4f aProjection, VREye aEye = VREye::None);


	void RenderTextureOnSlot(int aSlot, int aResourceSlot, PostProcessPass aPass, Ref<RenderTexture> aRenderTexture, ID3D11DepthStencilView* depthStencilView = nullptr);
	void RenderPass(PostProcessPass aPass);

	void Release();
	void ClearTargets();
	
};
