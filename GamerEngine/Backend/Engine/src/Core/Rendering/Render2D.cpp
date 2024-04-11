#include "GraphicsEngine.pch.h"
#include <Core/Rendering/Render2D.h>
#include <fstream>

#include "Renderer.h"
#include "Core/Framework/DX11.h"

void Render2D::Update()
{

}

void Render2D::Render()
{
	
}

void Render2D::InitializeShaders(int someSpriteAmmount)
{
	D3D11_INPUT_ELEMENT_DESC layout[] = {
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"ROTATION", 0, DXGI_FORMAT_R32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"SCALE", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"CENTER", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	std::ifstream vsFile;
	vsFile.open("Shaders\\Sprite_VS.cso", std::ios::binary);
	std::string vsData = { std::istreambuf_iterator(vsFile), std::istreambuf_iterator<char>() };
	DX11::Get().GetDevice()->CreateVertexShader(vsData.data(), vsData.size(), nullptr, myVertexShader.GetAddressOf());
	vsFile.close();

	std::ifstream gsFile;
	gsFile.open("Shaders\\Sprite_GS.cso", std::ios::binary);
	std::string gsData = { std::istreambuf_iterator(gsFile), std::istreambuf_iterator<char>() };
	DX11::Get().GetDevice()->CreateGeometryShader(gsData.data(), gsData.size(), nullptr, myGeometryShader.GetAddressOf());
	gsFile.close();

	std::ifstream psFile;
	psFile.open("Shaders\\Sprite_PS.cso", std::ios::binary);
	std::string psData = { std::istreambuf_iterator(psFile), std::istreambuf_iterator<char>() };
	DX11::Get().GetDevice()->CreatePixelShader(psData.data(), psData.size(), nullptr, myPixelShader.GetAddressOf());
	psFile.close();

	HRESULT res = DX11::Get().GetDevice()->CreateInputLayout(layout, sizeof(layout) / sizeof(D3D11_INPUT_ELEMENT_DESC), vsData.data(), vsData.size(), myInputLayout.GetAddressOf());

	myVertexStride = sizeof(SpriteVertex);
	myVertexOffset = 0;
	myPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;

	myVertices.resize(someSpriteAmmount);

	D3D11_BUFFER_DESC vertBufferDesc{};
	vertBufferDesc.ByteWidth = someSpriteAmmount * sizeof(SpriteVertex);
	vertBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA vertSubData{};
	vertSubData.pSysMem = &myVertices[0];

	DX11::Get().GetDevice()->CreateBuffer(&vertBufferDesc, &vertSubData, myVertexBuffer.GetAddressOf());
}
