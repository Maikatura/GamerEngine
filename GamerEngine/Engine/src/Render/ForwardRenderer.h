#pragma once
#include <Math/Matrix4x4.hpp>
#include <wrl.h>

#include <vector>
#include <Model/Model.h>
#include "RenderBuffer.h"
#include "RendererBase.h"
#include "Light/DirectionalLight.h"
#include "Light/EnvironmentLight.h"

#include "Render/Buffers.h"
#include <d3d11.h>

#include "Font/Font.h"

constexpr UINT MAX_FORWARD_LIGHTS = 20;


struct SceneLightBuffer
{
	Light::LightBufferData DirectionalLight;
	Light::LightBufferData LightsSpot[MAX_FORWARD_LIGHTS];
	Light::LightBufferData LightsPoint[MAX_FORWARD_LIGHTS];

	unsigned int NumLightsSpot;
	unsigned int NumLightsPoint;

	float Padding1;
	float Padding2;
};

class ForwardRenderer : public RendererBase
{
	
	SceneLightBuffer mySceneLightBufferData {};
	ComPtr<ID3D11Buffer> myLightBuffer;
	ComPtr<ID3D11Buffer> myTextBuffer;
	



public:


	bool Initialize();

	void Render(Matrix4x4f aView, Matrix4x4f aProjection, const std::vector<RenderBuffer>& aModelList, const Ref<DirectionalLight>& aDirectionalLight, const Ref<EnvironmentLight>& anEnvironmentLight, const std::vector<Light*>& aLightList, VREye anEye);

	void RenderSprites(Matrix4x4f aView, Matrix4x4f aProjection, std::vector<RenderBuffer2D>& aSpriteList, const Ref<DirectionalLight>& aDirectionalLight, const std::
		shared_ptr<EnvironmentLight>& anEnvironmentLight);


	void RenderString(const std::string& aString, Ref<Engine::Font> aFont, Matrix4x4f aTransform, Vector4f aColor);

	void BuildDepth();

	void ClearBuffer();

	void Release();
	
};

