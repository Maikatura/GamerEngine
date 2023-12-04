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

constexpr UINT MAX_DEFERRED_LIGHTS = 20;

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

	void SetAsTarget();
	void ClearTarget();
	void SetAsResource(unsigned int aStartSlot);
	void ClearResource(unsigned int aStartSlot);
	void Clear();

	void Release();

	bool CreateGBuffer();

	static RenderTexture& GetRenderer();

	static std::array<RenderTexture, GBufferTexture::GBufferTexture_Count>& GetPasses();


private:

	ID3D11Texture2D* myTexture;
	inline static std::array<RenderTexture, GBufferTexture::GBufferTexture_Count> myRenderTextures;

	inline static RenderTexture myRenderer;
};

class DeferredRenderer : public RendererBase
{
private:

	struct SceneLightBuffer
	{
		Light::LightBufferData DirectionalLight;
		Light::LightBufferData LightsSpot[MAX_DEFERRED_LIGHTS];
		Light::LightBufferData LightsPoint[MAX_DEFERRED_LIGHTS];

		unsigned int NumLightsSpot;
		unsigned int NumLightsPoint;
		
		float Padding1;
		float Padding2;
	} mySceneLightBufferData;

	ComPtr<ID3D11Buffer> myLightBuffer;

	ComPtr<ID3D11PixelShader> myGBufferPS;
	ComPtr<ID3D11PixelShader> myDeferredPS;
	ComPtr<ID3D11VertexShader> myDeferredVS;
	ComPtr<ID3D11PixelShader> myRenderTexPS;



public:


	bool Initialize();
	void GenerateGBuffer(Matrix4x4f aView, Matrix4x4f aProjection, const std::vector<RenderBuffer>& aModelList, float aDeltaTime, float aTotalTime, VREye anEye);
	void Render(Matrix4x4f aView, Matrix4x4f aProjection, const Ref<DirectionalLight>& aDirectionalLight, const Ref<EnvironmentLight>& anEnvironmentLight, std::vector<Light*> aLightList, float aDetlaTime, float aTotalTime, VREye anEye);
	void RenderLate();
	void ClearTarget();
};