#include "GraphicsEngine.pch.h"
#include "LineRenderer.h"
#include "GraphicsEngine.h"
#include "Framework/DX11.h"
#include <imgui/imgui.h>
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
	HRESULT result;

	/*LineVertex v[] = {
		LineVertex({0.0f,0.0f, -0.0f}),
		LineVertex({0.0f, 100.0f, -0.0f})
	};*/


	D3D11_SUBRESOURCE_DATA vertexBufferData;
	ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
	vertexBufferData.pSysMem = nullptr;


	D3D11_INPUT_ELEMENT_DESC layout[3] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "WIDTH", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	std::ifstream vsFile;
	vsFile.open("Shaders/LineshaderVS.cso", std::ios::binary);
	std::string vsData = { std::istreambuf_iterator<char>(vsFile), std::istreambuf_iterator<char>() };
	result = DX11::Device->CreateVertexShader(vsData.data(), vsData.size(), nullptr, myLineVertexShader.GetAddressOf());
	if(FAILED(result))
	{
		return false;
	}
	vsFile.close();

	std::ifstream psFile;
	psFile.open("Shaders/LineshaderPS.cso", std::ios::binary);
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

	return true;
}

void LineRenderer::DrawPoint(Vector3f aPosition, Vector4f aColor)
{
	std::array<LineVertex, 2> point = {
		LineVertex({aPosition.x,aPosition.y, aPosition.z, 1.0f }, aColor),
		LineVertex({aPosition.x,aPosition.y, aPosition.z, 1.0f }, aColor)
	};

	myLinesToRender.push_back(point);
}

void LineRenderer::DrawLine(Vector3f aStartPoint, Vector3f aEndPoint, Vector4f aColor)
{
	std::array<LineVertex, 2> lines = {
		LineVertex({aStartPoint.x,aStartPoint.y, aStartPoint.z, 1.0f }, aColor),
		LineVertex({aEndPoint.x,aEndPoint.y, aEndPoint.z, 1.0f }, aColor)
	};

	myLinesToRender.push_back(lines);
}

void LineRenderer::DrawCube(Vector3f aPosition, Vector3f aSize, Vector4f aColor)
{
	aPosition;
	aSize;
	aColor;
}

void LineRenderer::DrawCircle(Vector3f aPosition, float aRadius, int aTesselation)
{
	aPosition;
	aRadius;
	aTesselation;
	/*for(int i = 0; i < aTesselation; i++)
	{
		double angle = double(i) / double(aTesselation) * 3.14f * 2;
		Vector2f offset(sin(angle) * aRadius, cos(angle) * aRadius);
		DrawPoint({ aPosition.x + offset.x, aPosition.y + offset.y, 0.0f });
	}*/
}

void LineRenderer::Render()
{
	D3D11_MAPPED_SUBRESOURCE bufferData;

	myLineCBufferData.View = Matrix4x4f::GetFastInverse(Renderer::GetViewMatrix());
	myLineCBufferData.Projection = Renderer::GetProjectionMatrix();
	myLineCBufferData.World = Matrix4x4f();

	ZeroMemory(&bufferData, sizeof(D3D11_MAPPED_SUBRESOURCE));
	HRESULT result = DX11::Context->Map(myLineCBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferData);
	if(FAILED(result))
	{
		// BOOM?
		return;
	}

	memcpy(bufferData.pData, &myLineCBufferData, sizeof(LineCBufferData));
	DX11::Context->Unmap(myLineCBuffer.Get(), 0);

	UINT stride = sizeof(LineVertex);
	UINT offset = 0;

	// TODO : Skip the lines that is outside the camera (NOTE do it for models and will will have it for this too) (Also if they are out side the Far Plane distance on the camera skip them too)

	for(int i = 0; i < myLinesToRender.size(); i++)
	{
		DX11::Context->VSSetConstantBuffers(4, 1, myLineCBuffer.GetAddressOf());

		DX11::Context->IASetInputLayout(myInputLayout.Get());
		DX11::Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

		DX11::Context->VSSetShader(myLineVertexShader.Get(), NULL, 0);
		DX11::Context->PSSetShader(myLinePixelShader.Get(), NULL, 0);

		D3D11_MAPPED_SUBRESOURCE lineData;
		HRESULT hResult = DX11::Context->Map(myBuffer.Get(), 0,D3D11_MAP_WRITE_DISCARD, 0, &lineData);

		if(FAILED(hResult))
		{
			return;
		}

		memcpy(lineData.pData, myLinesToRender[i].data(), sizeof(LineVertex) * 2);
		DX11::Context->Unmap(myBuffer.Get(), 0);

		DX11::Context->IASetVertexBuffers(0, 1, myBuffer.GetAddressOf(), &stride, &offset);
		DX11::Context->Draw(2, 0);
	}

	myLinesToRender.clear();
}
