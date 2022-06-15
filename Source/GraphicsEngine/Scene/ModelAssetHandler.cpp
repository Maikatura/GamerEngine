#include "GraphicsEngine.pch.h"
#include "ModelAssetHandler.h"
#include <Model/Vertex.h>
#include "Frameworks/Framework_DX11.h"
#include <fstream>
#include <vector>

bool ModelAssetHandler::InitUnitCube() const
{
	HRESULT result = S_FALSE;

	std::vector<Vertex> mdlVertices =
	{
		{
			{-0.5f,-0.5f,-0.5f, 1.0f },
			{ {0.0f, 0.0f, 0.0f, 1.0f, }}

		},
		{
			{-0.5f,-0.5f, 0.5f, 1.0f},
			{{ 0.0f,0.0f,1.0f,1.0f }}
		},
		{
			{-0.5f, 0.5f,-0.5f,1.0f},
			{{0.0f,1.0f,0.0f,1.0f }}
		},
		{
			{-0.5f, 0.5f, 0.5f,1.0f},
			{ {1.0f,0.0f,1.0f,1.0f}}
		},
		{
			{0.5f,-0.5f,-0.5f,1.0f},
			{{0.0f,1.0f,1.0f,1.0f}}
		},
		{
			{0.5f,-0.5f, 0.5f,1.0f},
			{{1.0f,1.0f,0.0f,1.0f}}
		},
		{
			{ 0.5f, 0.5f,-0.5f,1.0f},
			{{1.0f,1.0f,1.0f,1.0f }}
		},
		{
			{ 0.5f, 0.5f, 0.5f,1.0f},
			{{0.0f,0.0f,0.0f,1.0f}}
		}
	};

	D3D11_BUFFER_DESC vertexBufferDesc = {};
	vertexBufferDesc.ByteWidth = static_cast<UINT>(mdlVertices.size()) * static_cast<UINT>(sizeof(Vertex));
	vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA vertexSubResourceData = {};
	vertexSubResourceData.pSysMem = &mdlVertices[0];

	ID3D11Buffer* vertexBuffer;
	result = DX11::Device->CreateBuffer(&vertexBufferDesc, &vertexSubResourceData, &vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	std::vector<unsigned int> mdlIndices =
	{
		0,2,1, // -x
		1,2,3,

		4,5,6, // +x
		5,7,6,

		0,1,5, // -y
		0,5,4,

		2,6,7, // +y
		2,7,3,

		0,4,6, // -z
		0,6,2,

		1,3,7, // +z
		1,7,5,
	};

	D3D11_BUFFER_DESC indexBufferDesc = {};
	indexBufferDesc.ByteWidth = static_cast<UINT>(mdlIndices.size()) * static_cast<UINT>(sizeof(unsigned int));
	indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA indexSubresourceData = {};
	indexSubresourceData.pSysMem = &mdlIndices[0];

	ID3D11Buffer* indexBuffer;
	result = DX11::Device->CreateBuffer(&indexBufferDesc, &indexSubresourceData, &indexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{"POSITION", 0 ,DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0 ,DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 1 ,DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 2 ,DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 3 ,DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},

	};

	std::ifstream vsFile;
	vsFile.open("DefaultVS.cso", std::ios::binary);
	std::string vsData = { std::istreambuf_iterator<char>(vsFile), std::istreambuf_iterator<char>() };
	ID3D11VertexShader* vertexShader;
	result = DX11::Device->CreateVertexShader(vsData.data(), vsData.size(), nullptr, &vertexShader);
	if (FAILED(result))
	{
		return false;
	}
	vsFile.close();

	std::ifstream psFile;
	psFile.open("DefaultPS.cso", std::ios::binary);
	std::string psData = { std::istreambuf_iterator<char>(psFile), std::istreambuf_iterator<char>() };
	ID3D11PixelShader* pixelShader;
	result = DX11::Device->CreatePixelShader(psData.data(), psData.size(), nullptr, &pixelShader);
	if (FAILED(result))
	{
		return false;
	}
	psFile.close();

	ID3D11InputLayout* inputLayout;
	result = DX11::Device->CreateInputLayout(layout, sizeof(layout) / sizeof(D3D11_INPUT_ELEMENT_DESC), vsData.data(), vsData.size(), &inputLayout);
	if (FAILED(result))
	{
		return false;
	}

	Model::MeshData modelData = {};
	modelData.myNumberOfVertices = static_cast<UINT>(mdlVertices.size());
	modelData.myNumberOfIndices = static_cast<UINT>(mdlIndices.size());
	modelData.myStride = sizeof(Vertex);
	modelData.myOffset = 0;
	modelData.myVertexBuffer = vertexBuffer;
	modelData.myIndexBuffer = indexBuffer;
	modelData.myVertexShader = vertexShader;
	modelData.myPixelShader = pixelShader;
	modelData.myPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	modelData.myInputLayout = inputLayout;

	std::shared_ptr<Model> mdl = std::make_shared<Model>();
	mdl->Init(modelData, L"Cube");
	myModelRegistry.insert({ L"Cube", mdl });


	return true;
}

bool ModelAssetHandler::InitUnitSphere() const
{
	

	return true;
}

bool ModelAssetHandler::Initialize() const
{
	if (!InitUnitCube())
	{
		return false;
	}

	if (!InitUnitSphere())
	{
		return false;
	}

	return true;
}

std::shared_ptr<Model> ModelAssetHandler::GetModel(const std::wstring& aFilePath) const
{
	auto model = myModelRegistry.find(aFilePath);
	return model->second;
}
