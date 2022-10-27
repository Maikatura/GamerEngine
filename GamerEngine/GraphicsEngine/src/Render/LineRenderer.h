#pragma once
#include <vector>
#include <Math/MathTypes.hpp>
#include <wrl/client.h>


// TODO : OPTIMIZE THE LINE RENDERER

struct ID3D11Buffer;
struct ID3D11InputLayout;
struct ID3D11VertexShader;
struct ID3D11PixelShader;

struct LineVertex
{
	Vector4f Position;
	Vector4f Color;
};

struct LineCBufferData
{
	Matrix4x4f World;
	Matrix4x4f View;
	Matrix4x4f Projection;
};


class LineRenderer
{
public:
	static bool Init();

	static void DrawPoint(Vector3f aPosition, Vector4f aColor = {1,1,1,1});
	static void DrawLine(Vector3f aStartPoint, Vector3f aEndPoint, Vector4f aColor = { 1,1,1,1 });
	static void DrawCube(Vector3f aPosition, Vector3f aSize, Vector4f aColor = { 1,1,1,1 });
	static void DrawCircle(Vector3f aPosition, float aRadius, int aTesselation = 24);


	static void Render();

private:

	static std::vector<std::array<LineVertex, 2>> myLinesToRender;

	static LineCBufferData myLineCBufferData;

	static Microsoft::WRL::ComPtr<ID3D11Buffer> myLineCBuffer;

	static Microsoft::WRL::ComPtr<ID3D11Buffer> myBuffer;
	static Microsoft::WRL::ComPtr<ID3D11InputLayout> myInputLayout;

	static Microsoft::WRL::ComPtr<ID3D11VertexShader> myLineVertexShader;
	static Microsoft::WRL::ComPtr<ID3D11PixelShader> myLinePixelShader;
};
