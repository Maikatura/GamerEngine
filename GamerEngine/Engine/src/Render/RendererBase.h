#pragma once
#include <wrl/client.h>
#include "Render/Buffers.h"
#include <d3d11.h>

class RendererBase
{
public:
	static bool Init();

	static void SetDepthStencilState(DepthStencilState aDepthStencilState);
	static void SetBlendState(BlendState aBlendState);
	static void SetSamplerState(int aSlot, SamplerState aSamplerState);

	void RemoveResource(int aSlot);

protected:
	FrameBufferData myFrameBufferData{};
	ObjectBufferData myObjectBufferData{};
	Vector4f myMaterialBufferData;
	/*BlendShapeData myBlendshapeBufferData{};*/
	
	Microsoft::WRL::ComPtr<ID3D11Buffer> myFrameBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> myObjectBuffer;
	//Microsoft::WRL::ComPtr<ID3D11Buffer> myBlendShapeBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> myMaterialBuffer;
	inline static Microsoft::WRL::ComPtr<ID3D11Buffer> myInstanceBuffer;
	inline static std::vector<Matrix4x4f> myInstancedTransformBufferData;

	inline static std::array<Microsoft::WRL::ComPtr<ID3D11DepthStencilState>, (int)DepthStencilState::Count> myDepthStencilStates;
	inline static std::array<Microsoft::WRL::ComPtr<ID3D11BlendState>, (int)BlendState::BSCount> myBlendStates;
	inline static std::array<Microsoft::WRL::ComPtr<ID3D11SamplerState>, (int)SamplerState::Count> mySamplerStates;
};
