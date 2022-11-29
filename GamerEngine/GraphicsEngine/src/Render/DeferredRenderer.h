#pragma once
#include <wrl/client.h>
#include <d3d11.h>
#include "Render/Buffers.h"
#include "RenderBuffer.h"
#include "RendererBase.h"

#include "Light/DirectionalLight.h"
#include "Light/EnvironmentLight.h"
#include "Light/SpotLight.h"
#include "Light/PointLight.h"

constexpr UINT MAX_DEFERRED_LIGHTS = 96;

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

	ID3D11Texture2D* myTexture;
	std::array<Microsoft::WRL::ComPtr<ID3D11RenderTargetView>, GBufferTexture::GB_COUNT> myRTVs;
	std::array<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>, GBufferTexture::GB_COUNT> mySRVs;
};

class DeferredRenderer : public RendererBase
{
private:

	struct SceneLightBuffer
	{
		Light::LightBufferData DirectionalLight;
		Light::LightBufferData Lights[MAX_DEFERRED_LIGHTS];

		unsigned int NumLights;
		Vector3f Padding;
	} mySceneLightBufferData;

	ComPtr<ID3D11Buffer> myLightBuffer;

	ComPtr<ID3D11PixelShader> myGBufferPS;
	ComPtr<ID3D11PixelShader> myDeferredPS;
	ComPtr<ID3D11VertexShader> myDeferredVS;
	ComPtr<ID3D11PixelShader> myRenderTexPS;

public:


	bool Initialize();
	void GenerateGBuffer(const std::vector<RenderBuffer>& aModelList, float aDeltaTime, float aTotalTime);
	void Render(const std::shared_ptr<DirectionalLight>& aDirectionalLight, const std::shared_ptr<EnvironmentLight>& anEnvironmentLight, std::vector<Light*> aLightList, float aDetlaTime, float aTotalTime);
	void RenderLate();

};