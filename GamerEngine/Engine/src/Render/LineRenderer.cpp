#include "GraphicsEngine.pch.h"
#include "LineRenderer.h"
#include "GraphicsEngine.h"
#include "Framework/DX11.h"
#include <d3d11.h>
#include <fstream>
#include "Render/Renderer.h"


ComPtr<ID3D11Buffer> LineRenderer::myLineCBuffer;
ComPtr<ID3D11Buffer> LineRenderer::myBuffer;
ComPtr<ID3D11InputLayout> LineRenderer::myInputLayout;

ComPtr<ID3D11VertexShader> LineRenderer::myLineVertexShader;
ComPtr<ID3D11PixelShader> LineRenderer::myLinePixelShader;

LineCBufferData LineRenderer::myLineCBufferData;
std::vector<std::array<LineVertex, 2>> LineRenderer::myLinesToRender;


bool LineRenderer::Init()
{
#if _DEBUG
	HRESULT result;

	D3D11_SUBRESOURCE_DATA vertexBufferData;
	ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
	vertexBufferData.pSysMem = nullptr;


	D3D11_INPUT_ELEMENT_DESC layout[3] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "WIDTH", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	std::ifstream vsFile;
	vsFile.open("Shaders\\Lineshader_VS.cso", std::ios::binary);
	std::string vsData = { std::istreambuf_iterator<char>(vsFile), std::istreambuf_iterator<char>() };
	result = DX11::Device->CreateVertexShader(vsData.data(), vsData.size(), nullptr, myLineVertexShader.GetAddressOf());
	if(FAILED(result))
	{
		return false;
	}
	vsFile.close();

	std::ifstream psFile;
	psFile.open("Shaders\\Lineshader_PS.cso", std::ios::binary);
	std::string psData = { std::istreambuf_iterator<char>(psFile), std::istreambuf_iterator<char>() };
	result = DX11::Device->CreatePixelShader(psData.data(), psData.size(), nullptr, myLinePixelShader.GetAddressOf());
	if(FAILED(result))
	{
		return false;
	}
	psFile.close();

	result = DX11::Device->CreateInputLayout(layout, sizeof(layout) / sizeof(D3D11_INPUT_ELEMENT_DESC), vsData.data(), vsData.size(), myInputLayout.GetAddressOf());
	if(FAILED(result))
	{
		return false;
	}

	D3D11_BUFFER_DESC bufferDescription = { 0 };
	bufferDescription.Usage = D3D11_USAGE_DYNAMIC;
	bufferDescription.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	bufferDescription.ByteWidth = sizeof(LineCBufferData);
	result = DX11::Device->CreateBuffer(&bufferDescription, nullptr, myLineCBuffer.GetAddressOf());
	if(FAILED(result))
	{
		return false;
	}

	bufferDescription.ByteWidth = sizeof(LineVertex) * 2;
	bufferDescription.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	result = DX11::Device->CreateBuffer(&bufferDescription, nullptr, myBuffer.GetAddressOf());
	if(FAILED(result))
	{
		return false;
	}
#endif
	return true;
}

void LineRenderer::DrawPoint(Vector3f aPosition, Vector4f aColor)
{
#if _DEBUG
	std::array<LineVertex, 2> point = {
		LineVertex({aPosition.x,aPosition.y, aPosition.z, 1.0f }, aColor),
		LineVertex({aPosition.x,aPosition.y, aPosition.z, 1.0f }, aColor)
	};

	myLinesToRender.push_back(point);
#endif
}

void LineRenderer::DrawLine(Vector3f aStartPoint, Vector3f aEndPoint, Vector4f aColor)
{
#if _DEBUG
	std::array<LineVertex, 2> lines = {
		LineVertex({aStartPoint.x,aStartPoint.y, aStartPoint.z, 1.0f }, aColor),
		LineVertex({aEndPoint.x,aEndPoint.y, aEndPoint.z, 1.0f }, aColor)
	};

	myLinesToRender.push_back(lines);
#endif
}

void LineRenderer::DrawCube(Vector3f aPosition, Vector3f aSize, Vector3f aRotation,  Vector4f aColor)
{
#if _DEBUG
	// Data
	float halfSizeX = (aSize.x * 0.5f);
	float halfSizeY = (aSize.y * 0.5f);
	float halfSizeZ = (aSize.z * 0.5f);

	Vector3f p1;
	Vector3f p2;
	Vector3f p3;
	Vector3f p4;
	Vector3f p5;
	Vector3f p6;
	Vector3f p7;
	Vector3f p8;

	LineVertex v1;
	LineVertex v2;
	LineVertex v3;
	LineVertex v4;


	aRotation *= 3.1415f / 180.0f;

	// Lower Half 
	{
		p1 = { aPosition.x + halfSizeX, aPosition.y - halfSizeY, aPosition.z + halfSizeZ };
		p2 = { aPosition.x + halfSizeX, aPosition.y - halfSizeY, aPosition.z - halfSizeZ };
		p3 = { aPosition.x - halfSizeX, aPosition.y - halfSizeY, aPosition.z - halfSizeZ };
		p4 = { aPosition.x - halfSizeX, aPosition.y - halfSizeY, aPosition.z + halfSizeZ };

		v1.Position = { p1.x , p1.y , p1.z, 1.0f };
		v1.Color = { aColor.x , aColor.y , aColor.z, aColor.w };

		v2.Position = { p2.x , p2.y , p2.z, 1.0f };
		v2.Color = { aColor.x , aColor.y , aColor.z, aColor.w };


		v3.Position = { p3.x , p3.y , p3.z , 1.0f };
		v3.Color = { aColor.x , aColor.y , aColor.z, aColor.w };

		v4.Position = { p4.x , p4.y , p4.z, 1.0f };
		v4.Color = { aColor.x , aColor.y , aColor.z, aColor.w };

		myLinesToRender.push_back({ v1, v2 });
		myLinesToRender.push_back({ v2, v3 });
		myLinesToRender.push_back({ v3, v4 });
		myLinesToRender.push_back({ v4, v1 });
	}
	// Top Half
	{
		p5 = { aPosition.x + halfSizeX , aPosition.y + halfSizeY, aPosition.z + halfSizeZ };
		p6 = { aPosition.x + halfSizeX , aPosition.y + halfSizeY, aPosition.z - halfSizeZ };
		p7 = { aPosition.x - halfSizeX , aPosition.y + halfSizeY, aPosition.z - halfSizeZ };
		p8 = { aPosition.x - halfSizeX , aPosition.y + halfSizeY, aPosition.z + halfSizeZ };


		v1.Position = { p5.x , p5.y , p5.z, 1.0f };
		v1.Color = { aColor.x , aColor.y , aColor.z, aColor.w };

		v2.Position = { p6.x , p6.y , p6.z , 1.0f };
		v2.Color = { aColor.x , aColor.y , aColor.z, aColor.w };

		v3.Position = { p7.x , p7.y , p7.z, 1.0f };
		v3.Color = { aColor.x , aColor.y , aColor.z, aColor.w };

		v4.Position = { p8.x , p8.y , p8.z , 1.0f };
		v4.Color = { aColor.x , aColor.y , aColor.z, aColor.w };

		myLinesToRender.push_back({ v1, v2 });
		myLinesToRender.push_back({ v2, v3 });
		myLinesToRender.push_back({ v3, v4 });
		myLinesToRender.push_back({ v4, v1 });
	}
	// Between Lower and Top
	{
		v1.Position = { p1.x , p1.y , p1.z, 1.0f };
		v1.Color = { aColor.x , aColor.y , aColor.z, aColor.w };

		v2.Position = { p5.x , p5.y , p5.z , 1.0f };
		v2.Color = { aColor.x , aColor.y , aColor.z, aColor.w };

		myLinesToRender.push_back({ v1, v2 });

		v1.Position = { p2.x , p2.y , p2.z , 1.0f };
		v1.Color = { aColor.x , aColor.y , aColor.z, aColor.w };

		v2.Position = { p6.x , p6.y , p6.z , 1.0f };
		v2.Color = { aColor.x , aColor.y , aColor.z, aColor.w };

		myLinesToRender.push_back({ v1, v2 });

		v1.Position = { p3.x , p3.y , p3.z , 1.0f };
		v1.Color = { aColor.x , aColor.y , aColor.z, aColor.w };

		v2.Position = { p7.x , p7.y , p7.z , 1.0f };
		v2.Color = { aColor.x , aColor.y , aColor.z, aColor.w };


		myLinesToRender.push_back({ v1, v2 });

		v1.Position = { p4.x , p4.y , p4.z, 1.0f };
		v1.Color = { aColor.x , aColor.y , aColor.z, aColor.w };

		v2.Position = { p8.x , p8.y , p8.z , 1.0f };
		v2.Color = { aColor.x , aColor.y , aColor.z, aColor.w };

		myLinesToRender.push_back({ v1, v2 });
	}
#endif
}

void LineRenderer::DrawCircle(Vector3f aPosition, float aRadius, int aTesselation)
{
	aPosition;
	aRadius;
	aTesselation;
#if _DEBUG
	/*for(int i = 0; i < aTesselation; i++)
	{
		double angle = double(i) / double(aTesselation) * 3.14f * 2;
		Vector2f offset(sin(angle) * aRadius, cos(angle) * aRadius);
		DrawPoint({ aPosition.x + offset.x, aPosition.y + offset.y, 0.0f });
	}*/
#endif
}



void LineRenderer::Render()
{
#if _DEBUG
	D3D11_MAPPED_SUBRESOURCE bufferData;

	myLineCBufferData.View = Matrix4x4f::GetFastInverse(Renderer::GetViewMatrix());
	myLineCBufferData.Projection = Renderer::GetProjectionMatrix();
	myLineCBufferData.World = Matrix4x4f();

	ZeroMemory(&bufferData, sizeof(D3D11_MAPPED_SUBRESOURCE));
	HRESULT result = DX11::GetContext()->Map(myLineCBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferData);
	if(FAILED(result))
	{
		// BOOM?
		return;
	}

	memcpy(bufferData.pData, &myLineCBufferData, sizeof(LineCBufferData));
	DX11::GetContext()->Unmap(myLineCBuffer.Get(), 0);

	UINT stride = sizeof(LineVertex);
	UINT offset = 0;

	// TODO : Skip the lines that is outside the camera (NOTE do it for models and will will have it for this too) (Also if they are out side the Far Plane distance on the camera skip them too)

	for(int i = 0; i < myLinesToRender.size(); i++)
	{
		DX11::GetContext()->VSSetConstantBuffers(3, 1, myLineCBuffer.GetAddressOf());

		DX11::GetContext()->IASetInputLayout(myInputLayout.Get());
		DX11::GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

		DX11::GetContext()->VSSetShader(myLineVertexShader.Get(), NULL, 0);
		DX11::GetContext()->PSSetShader(myLinePixelShader.Get(), NULL, 0);

		D3D11_MAPPED_SUBRESOURCE lineData;
		HRESULT hResult = DX11::GetContext()->Map(myBuffer.Get(), 0,D3D11_MAP_WRITE_DISCARD, 0, &lineData);

		if(FAILED(hResult))
		{
			return;
		}

		memcpy(lineData.pData, myLinesToRender[i].data(), sizeof(LineVertex) * 2);
		DX11::GetContext()->Unmap(myBuffer.Get(), 0);

		DX11::GetContext()->IASetVertexBuffers(0, 1, myBuffer.GetAddressOf(), &stride, &offset);
		DX11::GetContext()->Draw(2, 0);
	}
#endif
}

void LineRenderer::Clear()
{
#if _DEBUG
	myLinesToRender.clear();
#endif
}
