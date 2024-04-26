#pragma once
#include <vector>
#include <Math/MathTypes.hpp>
#include <wrl/client.h>
#include <d3d11.h>

#include "Core/RenderModule.h"
#include "Math/AABB3D.hpp"

// TODO : OPTIMIZE THE LINE RENDERER

struct ID3D11Buffer;
struct ID3D11InputLayout;
struct ID3D11VertexShader;
struct ID3D11PixelShader;

struct LineVertex
{
	LineVertex(Vector4f aPosition, Vector4f aColor, float aWidth = 1.0f)
	{
		Position = aPosition;
		Color = aColor;
		LineWidth = aWidth;
	}
	
	Vector4f Position;
	Vector4f Color;
	float LineWidth;
};

struct LineCBufferData
{
	Matrix4x4f World;
	Matrix4x4f View;
	Matrix4x4f Projection;
};


class LineRenderer : public RenderModule
{
public:


	static LineRenderer& Get();

	bool Init();

	bool OnAdd() override;
	void OnRelease() override;
	void OnRenderSetup() override;
	void OnUpdate() override;
	void OnRender() override;
	void OnEnd() override;

	void DrawPoint(Vector3f aPosition, Vector4f aColor = {1,1,1,1});
	void DrawLine(const Vector3f& aStartPoint, const Vector3f& aEndPoint, const Vector4f& aColor = { 1,1,1,1 }, float aWidth = 1.0f);
	void DrawCube(Vector3f aPosition, Vector3f aSize, Vector3f aRotation = { 0,0,0 }, Vector4f aColor = { 1,1,1,1 });
	void DrawAABB3D(const CommonUtilities::AABB3D<float>& aAABB, Vector4f aColor = { 1,1,1,1 });
	void DrawCircle(Vector3f aPosition, float aRadius, int aTesselation = 24) const;

	

private:

	void Update();
	void Render(const Matrix4x4f& aView, const Matrix4x4f& aProjection);
	void Clear();

	inline static LineRenderer* myInstance = nullptr;

	bool myLineRendererOn = false;
	
	std::vector<std::array<LineVertex, 2>> myLinesToRender;

	LineCBufferData myLineCBufferData;

	Microsoft::WRL::ComPtr<ID3D11Buffer> myLineCBuffer;

	Microsoft::WRL::ComPtr<ID3D11Buffer> myBuffer;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> myInputLayout;

	Microsoft::WRL::ComPtr<ID3D11VertexShader> myLineVertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> myLinePixelShader;
	Microsoft::WRL::ComPtr<ID3D11GeometryShader> myLineGeometryShader;
};