#pragma once

#include <wrl/client.h>

#include "Buffers.h"
#include <d3d11.h>

#include "RenderBuffer.h"
#include "RendererBase.h"
#include "Light/DirectionalLight.h"
#include "Light/Light.h"

using namespace Microsoft::WRL;

class ShadowRenderer : public RenderModule
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

public:

	bool OnAdd() override;
	void OnRelease() override;
	void OnUpdate() override;
	void OnRenderSetup() override;
	void OnRender() override;
	void OnEnd() override;


	bool Initialize();

	void Render(Light* aLight, const std::vector<GamerEngine::RenderBuffer>& aModelList);
	static void ClearResources();
	static void ClearTarget();
};