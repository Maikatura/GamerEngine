#pragma once

#include <wrl/client.h>

#include "Renderer/Render/Buffers.h"
#include <d3d11.h>

#include "RenderBuffer.h"
#include "RendererBase.h"
#include "Renderer/Light/DirectionalLight.h"
#include "Renderer/Light/Light.h"

using namespace Microsoft::WRL;

class ShadowRenderer : public RendererBase
{
	struct PointLightView
	{
		Vector3f SC_LightTranslation;
		float SC_FarPlane;
		Matrix4x4f SC_Projection;
		Matrix4x4f myPointLightViews[6];

	} myPointLightView;

	ComPtr<ID3D11Buffer> myPointLightBuffer;

	ComPtr<ID3D11GeometryShader> myShadowGeometryShader;
	ComPtr<ID3D11VertexShader> myDeferredVS;
	ComPtr<ID3D11PixelShader> myDeferredPS;

public:
	bool Initialize();

	void Render(Light* aLight, const std::vector<RenderBuffer>& aModelList);
	void ClearResources();
	void ClearTarget();
};