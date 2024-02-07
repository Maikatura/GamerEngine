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
	struct ShadowCubeBuffer
	{
		Vector3f SC_LightTranslation;
		float SC_FarPlane;
		Matrix4x4f SC_Projection;
		Matrix4x4f SC_Views[6];

	} myShadowCubeData;

	ComPtr<ID3D11Buffer> myPointLightBuffer;

	ComPtr<ID3D11GeometryShader> myShadowGeometryShader;
	ComPtr<ID3D11VertexShader> myDeferredVS;
	ComPtr<ID3D11PixelShader> myDeferredPS;

public:
	bool Initialize();

	void Render(Light* aLight, const std::vector<RenderBuffer>& aModelList);
	static void ClearResources();
	static void ClearTarget();
};