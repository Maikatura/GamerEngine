#pragma once

#include <wrl/client.h>

#include "Render/Buffers.h"
#include <d3d11.h>

#include "RenderBuffer.h"
#include "RendererBase.h"
#include "Light/DirectionalLight.h"
#include "Light/Light.h"

using namespace Microsoft::WRL;

class ShadowRenderer : public RendererBase
{
	struct PointLightView
	{
		Matrix4x4f myPointLightViews[6];
	} myPointLightView;

	ComPtr<ID3D11Buffer> myPointLightBuffer;

	ComPtr<ID3D11GeometryShader> myShadowGeometryShader;

public:
	bool Initialize();

	void Render(const std::vector<RenderBuffer>& aModelList);
	void ClearResources();
};