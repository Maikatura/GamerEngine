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

constexpr UINT MAX_FORWARD_LIGHTS = 8;


struct SceneLightBuffer
{
	Light::LightBufferData DirectionalLight;
	Light::LightBufferData Lights[MAX_FORWARD_LIGHTS];

	unsigned int NumLights;
	Vector3f Padding;
};

class ForwardRenderer : public RendererBase
{
	
	SceneLightBuffer mySceneLightBufferData {};
	Microsoft::WRL::ComPtr<ID3D11Buffer> myLightBuffer;

public:


	bool Initialize();

	void Render(const std::vector<RenderBuffer>& aModelList, const std::shared_ptr<DirectionalLight>& aDirectionalLight, const std::
	            shared_ptr<EnvironmentLight>& anEnvironmentLight, const std::vector<Light*>& aLightList);

	void RenderSprites(std::vector<RenderBuffer2D>& aSpriteList, const std::shared_ptr<DirectionalLight>& aDirectionalLight, const std::
		shared_ptr<EnvironmentLight>& anEnvironmentLight);

	void BuildDepth();

	void ClearBuffer();

	void Release();
	
};

