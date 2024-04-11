#include "GraphicsEngine.pch.h"
#include "LineRenderer.h"
#include "GraphicsEngine.h"
#include "Core/Framework/DX11.h"
#include <d3d11.h>
#include <fstream>
#include "Core/Rendering/Renderer.h"


LineRenderer& LineRenderer::Get()
{
	static LineRenderer instance;
	return instance;
}

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
		{ "WIDTH", 0, DXGI_FORMAT_R32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	std::ifstream vsFile;
	vsFile.open("Shaders\\Lineshader_VS.cso", std::ios::binary);
	std::string vsData = { std::istreambuf_iterator<char>(vsFile), std::istreambuf_iterator<char>() };
	result = DX11::Get().GetDevice()->CreateVertexShader(vsData.data(), vsData.size(), nullptr, myLineVertexShader.GetAddressOf());
	if(FAILED(result))
	{
		return false;
	}
	vsFile.close();

	std::ifstream psFile;
	psFile.open("Shaders\\Lineshader_PS.cso", std::ios::binary);
	std::string psData = { std::istreambuf_iterator<char>(psFile), std::istreambuf_iterator<char>() };
	result = DX11::Get().GetDevice()->CreatePixelShader(psData.data(), psData.size(), nullptr, myLinePixelShader.GetAddressOf());
	if(FAILED(result))
	{
		return false;
	}
	psFile.close();

	std::ifstream gsFile;
	gsFile.open("Shaders\\Lineshader_GS.cso", std::ios::binary);
	std::string gsData = { std::istreambuf_iterator<char>(gsFile), std::istreambuf_iterator<char>() };
	result = DX11::Get().GetDevice()->CreateGeometryShader(gsData.data(), gsData.size(), nullptr, myLineGeometryShader.GetAddressOf());
	if(FAILED(result))
	{
		return false;
	}
	gsFile.close();
	
	result = DX11::Get().GetDevice()->CreateInputLayout(layout, sizeof(layout) / sizeof(D3D11_INPUT_ELEMENT_DESC), vsData.data(), vsData.size(), myInputLayout.GetAddressOf());
	if(FAILED(result))
	{
		return false;
	}

	D3D11_BUFFER_DESC bufferDescription = { 0 };
	bufferDescription.Usage = D3D11_USAGE_DYNAMIC;
	bufferDescription.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	bufferDescription.ByteWidth = sizeof(LineCBufferData);
	result = DX11::Get().GetDevice()->CreateBuffer(&bufferDescription, nullptr, myLineCBuffer.GetAddressOf());
	if(FAILED(result))
	{
		return false;
	}

	bufferDescription.ByteWidth = sizeof(LineVertex) * 2;
	bufferDescription.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	result = DX11::Get().GetDevice()->CreateBuffer(&bufferDescription, nullptr, myBuffer.GetAddressOf());
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

void LineRenderer::DrawLine(const Vector3f& aStartPoint, const Vector3f& aEndPoint, const Vector4f& aColor, float aWidth)
{
#if _DEBUG
	std::array<LineVertex, 2> lines = {
		LineVertex({aStartPoint.x,aStartPoint.y, aStartPoint.z, 1.0f }, aColor, aWidth),
		LineVertex({aEndPoint.x,aEndPoint.y, aEndPoint.z, 1.0f }, aColor, aWidth)
	};

	myLinesToRender.push_back(lines);
#endif
}

void LineRenderer::DrawCube(Vector3f aPosition, Vector3f aSize, Vector3f aRotation,  Vector4f aColor)
{
#if _DEBUG

		// Convert rotation from degrees to radians
		aRotation *= 3.1415f / 180.0f;

		// Create rotation matrix
		Matrix4x4f rotationMatrix = Matrix4x4f::CreateRotationAroundX(aRotation.x) * Matrix4x4f::CreateRotationAroundY(aRotation.y) * Matrix4x4f::CreateRotationAroundZ(aRotation.z);

		// Define the 8 points of the cube
		Vector3f halfSize = aSize * 0.5f;
		std::array<Vector3f, 8> points = {
			aPosition + Vector3f(-halfSize.x, -halfSize.y, -halfSize.z),
			aPosition + Vector3f(halfSize.x, -halfSize.y, -halfSize.z),
			aPosition + Vector3f(halfSize.x, -halfSize.y, halfSize.z),
			aPosition + Vector3f(-halfSize.x, -halfSize.y, halfSize.z),
			aPosition + Vector3f(-halfSize.x, halfSize.y, -halfSize.z),
			aPosition + Vector3f(halfSize.x, halfSize.y, -halfSize.z),
			aPosition + Vector3f(halfSize.x, halfSize.y, halfSize.z),
			aPosition + Vector3f(-halfSize.x, halfSize.y, halfSize.z)
		};

		// Apply rotation to each point
		for (Vector3f& point : points) {
			Vector3f relativePosition = point - aPosition;
			Vector3f rotatedPosition = rotationMatrix * relativePosition;
			point = aPosition + rotatedPosition;
		}

		// Draw the 12 edges of the cube
		for (int i = 0; i < 4; ++i) {
			DrawLine(points[i], points[(i + 1) % 4], aColor); // Bottom square
			DrawLine(points[i + 4], points[((i + 1) % 4) + 4], aColor); // Top square
			DrawLine(points[i], points[i + 4], aColor); // Vertical lines
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

void LineRenderer::Render(const Matrix4x4f& aView,const Matrix4x4f& aProjection)
{
#if _DEBUG
	D3D11_MAPPED_SUBRESOURCE bufferData;

	myLineCBufferData.View = Matrix4x4f::GetFastInverse(aView);
	myLineCBufferData.Projection = aProjection;
	myLineCBufferData.World = Matrix4x4f();

	ZeroMemory(&bufferData, sizeof(D3D11_MAPPED_SUBRESOURCE));
	HRESULT result = DX11::Get().GetContext()->Map(myLineCBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferData);
	if (FAILED(result))
	{
		// BOOM?
		return;
	}

	memcpy(bufferData.pData, &myLineCBufferData, sizeof(LineCBufferData));
	DX11::Get().GetContext()->Unmap(myLineCBuffer.Get(), 0);

	DX11::Get().GetContext()->VSSetConstantBuffers(6, 1, myLineCBuffer.GetAddressOf());

	UINT stride = sizeof(LineVertex);
	UINT offset = 0;

	// TODO : Skip the lines that is outside the camera (NOTE do it for models and will will have it for this too) (Also if they are out side the Far Plane distance on the camera skip them too)

	for (int i = 0; i < myLinesToRender.size(); i++)
	{

		DX11::Get().GetContext()->IASetInputLayout(myInputLayout.Get());
		DX11::Get().GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

		DX11::Get().GetContext()->VSSetShader(myLineVertexShader.Get(), NULL, 0);
		//DX11::Get().GetContext()->GSSetShader(myLineGeometryShader.Get(), NULL, 0);
		DX11::Get().GetContext()->PSSetShader(myLinePixelShader.Get(), NULL, 0);

		D3D11_MAPPED_SUBRESOURCE lineData;
		HRESULT hResult = DX11::Get().GetContext()->Map(myBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &lineData);

		if (FAILED(hResult))
		{
			return;
		}

		memcpy(lineData.pData, myLinesToRender[i].data(), sizeof(LineVertex) * 2);
		DX11::Get().GetContext()->Unmap(myBuffer.Get(), 0);

		DX11::Get().GetContext()->IASetVertexBuffers(0, 1, myBuffer.GetAddressOf(), &stride, &offset);
		DX11::Get().GetContext()->Draw(2, 0);
	}

	Clear();
#endif
}

void LineRenderer::Clear()
{
#if _DEBUG
	myLinesToRender.clear();
#endif
}
