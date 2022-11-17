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
	static void SetSamplerState(SamplerState aSamplerState, int aStartSlot);

protected:
	FrameBufferData myFrameBufferData{};
	ObjectBufferData myObjectBufferData{};

	Microsoft::WRL::ComPtr<ID3D11Buffer> myFrameBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> myObjectBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> myMaterialBuffer;

	inline static std::array<Microsoft::WRL::ComPtr<ID3D11DepthStencilState>, (int)DepthStencilState::DSS_Count> myDepthStencilStates;
	inline static std::array<Microsoft::WRL::ComPtr<ID3D11BlendState>, (int)BlendState::BSCount> myBlendStates;
	inline static std::array<Microsoft::WRL::ComPtr<ID3D11SamplerState>, (int)SamplerState::SS_Count> mySamplerStates;
};
