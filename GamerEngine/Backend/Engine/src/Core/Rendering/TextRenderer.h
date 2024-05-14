#pragma once
#include <wrl/client.h>
#include "RenderBuffer.h"
#include "Core/RenderModule.h"


namespace GamerEngine
{
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
		Microsoft::WRL::ComPtr<ID3D11VertexShader> myTextVS;
		Microsoft::WRL::ComPtr<ID3D11PixelShader> myTextPS;
	};

}