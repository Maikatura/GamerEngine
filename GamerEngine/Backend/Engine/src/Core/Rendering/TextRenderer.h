#pragma once
#include "Renderer.h"
#include "Core/RenderModule.h"


struct TextVertexBufferData
{
	TextVertex Vertex[4];
};

class TextRenderer : public RenderModule
{
public:
	bool OnAdd() override;
	void OnRelease() override;

	void OnUpdate() override;
	void OnRenderSetup() override;
	void OnRender() override;
	void OnEnd() override;



private:
	TextVertexBufferData myTextBufferData{};

	Microsoft::WRL::ComPtr<ID3D11Buffer> myTextBuffer;

	ComPtr<ID3D11VertexShader> myTextVS;
	ComPtr<ID3D11PixelShader> myTextPS;
};
