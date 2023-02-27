#pragma once
#include <wrl.h>
#include <d3d11.h>

using Microsoft::WRL::ComPtr;

struct ModelData 
{
	ComPtr<ID3D11Buffer> myVertexBuffer;
	ComPtr<ID3D11Buffer> myIndexBuffer;

	UINT myPrimitiveTopology;
	UINT myNumberOfVertices;
	UINT myNumberOfIndices;
	UINT myStride;
	UINT myOffset;

	ComPtr<ID3D11InputLayout> myInputLayout;
	ComPtr<ID3D11VertexShader> myVertexShader;
	ComPtr<ID3D11PixelShader> myPixelShader;
};