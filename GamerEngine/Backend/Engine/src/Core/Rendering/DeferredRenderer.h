#pragma once
#include <wrl/client.h>
#include <d3d11.h>
#include "Buffers.h"
#include "RenderBuffer.h"
#include "RendererBase.h"
#include "Light/DirectionalLight.h"
#include "Light/EnvironmentLight.h"

constexpr UINT MAX_DEFERRED_LIGHTS = 20;

class GBuffer
{
		friend class TextureAssetHandler;

	public:
		enum GBufferTexture
		{
			EGBufferTexture_Albedo,
			EGBufferTexture_Normal,
			EGBufferTexture_Material,
			EGBufferTexture_VertexNormal,
			EGBufferTexture_Position,
			EGBufferTexture_AmbientOcclusion,
			EGBufferTexture_ViewPosition,
			EGBufferTexture_ViewNormal,
			EGBufferTexture_Count
		};

		static void SetAsTarget();
		static void ClearTarget();
		static void SetAsResource(unsigned int aStartSlot);
		static void ClearResource(unsigned int aStartSlot);
		static void Clear();

		static void Release();

		static bool CreateGBuffer();

		static RenderTexture& GetRenderer();

		static std::array<RenderTexture, GBufferTexture::EGBufferTexture_Count>& GetPasses();
	
	private:

		ID3D11Texture2D* myTexture;
		inline static std::array<RenderTexture, GBufferTexture::EGBufferTexture_Count> myRenderTextures;

		inline static RenderTexture myRenderer;
};

class DeferredRenderer : public RendererBase
{
	public:
		bool Initialize();
		void GenerateGBuffer(Matrix4x4f aView, const Matrix4x4f& aProjection, const std::vector<RenderBuffer>& aModelList, float aDeltaTime, float aTotalTime, VREye anEye);
		void Render(Matrix4x4f aView, const Matrix4x4f& aProjection, const Ref<DirectionalLight>& aDirectionalLight, const Ref<EnvironmentLight>& anEnvironmentLight, const std::vector<Light*>& aLightList, float aDeltaTime, float aTotalTime, VREye anEye);
		void RenderLate() const;
		static void ClearTarget();

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
		};
		
		SceneLightBuffer mySceneLightBufferData = {};
		
		ComPtr<ID3D11Buffer> myLightBuffer;

		ComPtr<ID3D11PixelShader> myGBufferPS;
		ComPtr<ID3D11PixelShader> myDeferredPS;
		ComPtr<ID3D11VertexShader> myDeferredVS;
		ComPtr<ID3D11PixelShader> myRenderTexPS;
};