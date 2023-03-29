#pragma once
#include <memory>

#include "Renderer/Model/Texture.h"
#include <Renderer/Model/Render2D.h>
#include <wrl/client.h>
#include <d3d11.h>

class Render2D
{
public:
	void Update();
	virtual void SetAsResource() = 0;

protected:
	Render2D();
	void Render();
	void InitializeShaders(int someSpriteAmmount = 1);
	std::shared_ptr<Texture> myTexture;
	std::vector<SpriteVertex> myVertices;
	ComPtr<ID3D11Buffer> myVertexBuffer;
	UINT myVertexStride;
	UINT myVertexOffset;
	ComPtr<ID3D11VertexShader> myVertexShader;
	ComPtr<ID3D11GeometryShader> myGeometryShader;
	ComPtr<ID3D11PixelShader> myPixelShader;
	UINT myPrimitiveTopology;
	ComPtr<ID3D11InputLayout> myInputLayout;
};

