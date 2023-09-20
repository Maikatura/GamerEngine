#pragma once
#include <Math/Matrix4x4.hpp>
#include <wrl.h>

#include <vector>
#include <Renderer/Model/Model.h>
#include "RenderBuffer.h"
#include "RendererBase.h"
#include "Renderer/Light/DirectionalLight.h"
#include "Renderer/Light/EnvironmentLight.h"

#include "Renderer/Render/Buffers.h"
#include <d3d11.h>

constexpr UINT MAX_FORWARD_LIGHTS = 98;


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
	ComPtr<ID3D11Buffer> myLightBuffer;
	



public:


	bool Initialize();

	void Render(Matrix4x4f aView, Matrix4x4f aProjection,const std::vector<RenderBuffer>& aModelList, const std::shared_ptr<DirectionalLight>& aDirectionalLight, const std::
	            shared_ptr<EnvironmentLight>& anEnvironmentLight, const std::vector<Light*>& aLightList, VREye anEye);

	void RenderSprites(std::vector<RenderBuffer2D>& aSpriteList, const std::shared_ptr<DirectionalLight>& aDirectionalLight, const std::
		shared_ptr<EnvironmentLight>& anEnvironmentLight);

	void BuildDepth();

	void ClearBuffer();

	void Release();
	
};

