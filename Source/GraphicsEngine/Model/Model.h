#pragma once
#include <wrl.h>
#include <string>
#include <Model/SceneObject.h>

using Microsoft::WRL::ComPtr;

class Model : public SceneObject
{
public:
	struct MeshData 
	{
		UINT myNumberOfVertices;
		UINT myNumberOfIndices;
		UINT myStride;
		UINT myOffset;
		ComPtr<ID3D11Buffer> myVertexBuffer;
		ComPtr<ID3D11Buffer> myIndexBuffer;
		ComPtr<ID3D11VertexShader> myVertexShader;
		ComPtr<ID3D11PixelShader> myPixelShader;
		ComPtr<ID3D11InputLayout> myInputLayout;
		UINT myPrimitiveTopology;
	};

private:

	MeshData myMeshData = {};
	std::wstring myPath;

public:
	void Init(MeshData aMeshData, const std::wstring& aPath) 
	{
		myMeshData = aMeshData;
		myPath = aPath;
	}

	FORCEINLINE MeshData const& GetMeshData() const { return myMeshData; }
	FORCEINLINE std::wstring const& GetName() const { return myPath; }
};

