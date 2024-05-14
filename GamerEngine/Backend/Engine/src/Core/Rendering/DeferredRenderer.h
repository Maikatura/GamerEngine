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

		void SetAsTarget();
		void SetAsResource(unsigned int aStartSlot);
		void ClearResource(unsigned int aStartSlot);
		void Clear();

		void Release();

		bool CreateGBuffer();
		RenderTexture& GetShaderResourceView()
		{
			return myRenderer;
		};

		//RenderTexture& GetRenderer();

	private:

		ID3D11Texture2D* myTexture;
		std::array<RenderTexture, GBufferTexture::EGBufferTexture_Count> myRenderTextures;
		RenderTexture myRenderer;
};

class DeferredRenderer : public RenderModule
{
	public:

		bool OnAdd() override;
		void OnRelease() override;

		void OnUpdate() override;
		void OnRenderSetup() override;
		void OnRender() override;
		void OnEnd() override;
	
		bool Initialize();
		void GenerateGBuffer(Matrix4x4f aView, const Matrix4x4f& aProjection, const std::vector<GamerEngine::RenderBuffer>& aModelList, float aDeltaTime, float aTotalTime, VREye anEye);
		void Render(Matrix4x4f aView, const Matrix4x4f& aProjection, const Ref<DirectionalLight>& aDirectionalLight, const Ref<EnvironmentLight>& anEnvironmentLight, const std::vector<Light*>& aLightList, float aDeltaTime, float aTotalTime, VREye anEye);
		void ClearTarget();

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

		Ref<GBuffer> myGBuffer;
	
		SceneLightBuffer mySceneLightBufferData = {};
		FrameBufferData myFrameBufferData{};
		ObjectBufferData myObjectBufferData{};
		Vector4f myMaterialBufferData;
		/*BlendShapeData myBlendshapeBufferData{};*/
		
		Microsoft::WRL::ComPtr<ID3D11Buffer> myFrameBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer> myObjectBuffer;
		//Microsoft::WRL::ComPtr<ID3D11Buffer> myBlendShapeBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer> myMaterialBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer> myInstanceBuffer;
		std::vector<Matrix4x4f> myInstancedTransformBufferData;
		ComPtr<ID3D11Buffer> myLightBuffer;

		ComPtr<ID3D11PixelShader> myGBufferPS;
		ComPtr<ID3D11PixelShader> myDeferredPS;
		ComPtr<ID3D11VertexShader> myDeferredVS;
		ComPtr<ID3D11PixelShader> myRenderTexPS;
};