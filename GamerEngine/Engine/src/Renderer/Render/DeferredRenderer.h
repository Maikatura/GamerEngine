#pragma once
#include <wrl/client.h>
#include <d3d11.h>
#include "Renderer/Render/Buffers.h"
#include "RenderBuffer.h"
#include "RendererBase.h"

#include "Renderer/Light/DirectionalLight.h"
#include "Renderer/Light/EnvironmentLight.h"
#include "Renderer/Light/SpotLight.h"
#include "Renderer/Light/PointLight.h"

constexpr UINT MAX_DEFERRED_LIGHTS = 96;

class GBuffer
{
	friend class TextureAssetHandler;

public:
	enum GBufferTexture
	{
		GBufferTexture_Albedo,
		GBufferTexture_Normal,
		GBufferTexture_Material,
		GBufferTexture_VertexNormal,
		GBufferTexture_Position,
		GBufferTexture_AmbientOcclusion,
		GBufferTexture_ViewPosition,
		GBufferTexture_ViewNormal,
		GBufferTexture_Count
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
	std::array<Microsoft::WRL::ComPtr<ID3D11RenderTargetView>, GBufferTexture::GBufferTexture_Count> myRTVs;
	std::array<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>, GBufferTexture::GBufferTexture_Count> mySRVs;
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
	void Render(Matrix4x4f aView, Matrix4x4f aProjection, const std::shared_ptr<DirectionalLight>& aDirectionalLight, const std::shared_ptr<EnvironmentLight>& anEnvironmentLight, std::vector<Light*> aLightList, float aDetlaTime, float aTotalTime);
	void RenderLate();
	void ClearTarget();
};