#pragma once
#include "RendererBase.h"

class PostProcessRenderer : public RendererBase
{
public:
	enum PostProcessPass
	{
		PP_COPY,
		PP_LUMINANCE,
		PP_GAUSSIAN,
		PP_BLOOM,
		PP_COUNT
	};

private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> myFullscreenVS;

	std::array<Microsoft::WRL::ComPtr<ID3D11PixelShader>, PostProcessPass::PP_COUNT> myPassShaders;

public:

	bool Initialize();

	void Render(PostProcessPass aPass);

};
