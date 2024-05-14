#include "GraphicsEngine.pch.h"
#include "TextRenderer.h"

#include "Renderer.h"

bool GamerEngine::TextRenderer::OnAdd()
{
	D3D11_BUFFER_DESC bufferDescription;
	ZeroMemory(&bufferDescription, sizeof(D3D11_BUFFER_DESC));
	bufferDescription.Usage = D3D11_USAGE_DYNAMIC;
	bufferDescription.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	bufferDescription.ByteWidth = sizeof(TextVertexBufferData);
	HRESULT result = DX11::Get().GetDevice()->CreateBuffer(&bufferDescription, nullptr, myTextBuffer.GetAddressOf());
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void GamerEngine::TextRenderer::OnRelease()
{
}

void GamerEngine::TextRenderer::OnUpdate()
{
}

void GamerEngine::TextRenderer::OnRenderSetup()
{
}

void GamerEngine::TextRenderer::OnRender()
{
	return;
	auto& textVertex = GamerEngine::Renderer::GetText();

	D3D11_MAPPED_SUBRESOURCE bufferData;


	for (int i = 0; i < textVertex.size(); i += 4)
	{
		myTextBufferData.Vertex[0] = textVertex[i + 0];
		myTextBufferData.Vertex[1] = textVertex[i + 1];
		myTextBufferData.Vertex[2] = textVertex[i + 2];
		myTextBufferData.Vertex[3] = textVertex[i + 3];

		ZeroMemory(&bufferData, sizeof(D3D11_MAPPED_SUBRESOURCE));
		HRESULT result = DX11::Get().GetContext()->Map(myTextBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferData);
		if (FAILED(result))
		{
			// BOOM?
			return;
		}

		memcpy_s(bufferData.pData, sizeof(TextVertexBufferData), &myTextBufferData, sizeof(TextVertexBufferData));
		DX11::Get().GetContext()->Unmap(myTextBuffer.Get(), 0);

		DX11::Get().GetContext()->VSSetConstantBuffers(0, 1, myTextBuffer.GetAddressOf());
		DX11::Get().GetContext()->PSSetConstantBuffers(0, 1, myTextBuffer.GetAddressOf());

		
	}

	


}

void GamerEngine::TextRenderer::OnEnd()
{
	
}
