#pragma once
#include <wrl/client.h>
#include <d3d11.h>

#include "ForwardRenderer.h"
#include "RenderBuffer.h"

#include "Light/DirectionalLight.h"
#include "Light/EnvironmentLight.h"

class GBuffer
{
	friend class TextureAssetHandler;

public:
	enum GBufferTexture
	{
		GB_ALBEDO,
		GB_NORMAL,
		GB_MATERIAL,
		GB_VERTEXNORMAL,
		GB_POSITION,
		GB_AMBIENTOCCLUSION,
		GB_COUNT
	};

	void SetAsTarget() const;
	void ClearTarget() const;
	void SetAsResource(unsigned int aStartSlot) const;
	void ClearResource(unsigned int aStartSlot) const;
	void Clear();

	void Release();

	bool CreateGBuffer();

private:
	std::array<Microsoft::WRL::ComPtr<ID3D11RenderTargetView>, GBufferTexture::GB_COUNT> myRTVs;
	std::array<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>, GBufferTexture::GB_COUNT> mySRVs;
};

class DeferredRenderer
{
private:
	FrameBufferData myFrameBufferData{};
	ObjectBufferData myObjectBufferData{};

	ComPtr<ID3D11Buffer> myFrameBuffer;
	ComPtr<ID3D11Buffer> myObjectBuffer;
	ComPtr<ID3D11Buffer> myMaterialBuffer;
	ComPtr<ID3D11Buffer> myLightBuffer;

	ComPtr<ID3D11PixelShader> myGBufferPS;
	ComPtr<ID3D11PixelShader> myFullscreenPS;
	ComPtr<ID3D11VertexShader> myFullscreenVS;
public:


	bool Init();
	void GenerateGBuffer(const std::vector<RenderBuffer>& aModelList, float aDeltaTime, float aTotalTime);
	void Render(const std::shared_ptr<DirectionalLight>& aDirectionalLight, const std::shared_ptr<EnvironmentLight>& anEnvironmentLight, float aDetlaTime, float aTotalTime);
};