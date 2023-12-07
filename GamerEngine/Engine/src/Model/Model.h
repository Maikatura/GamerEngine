#pragma once
#include <string>
#include <utility>
#include <vector>
#include <Model/SceneObject.h>
#include <unordered_map>
#include <Windows.h>
#include <d3d11.h>
#include <Math/MathTypes.hpp>
#include "Material.h"
#include <EngineSettings/Settings.h>

#include "Vertex.h"

using namespace Microsoft::WRL;



class Model : public SceneObject
{
public:

	/*struct BlendShapeData
	{
		std::vector<Vertex> BlendShapeVertex;
		std::string Name;
		float Value = 0.0f;
	};

	struct MeshData 
	{
		ComPtr<ID3D11Buffer> myVertexBuffer;
		ComPtr<ID3D11Buffer> myIndexBuffer;
		ComPtr<ID3D11VertexShader> myVertexShader;
		ComPtr<ID3D11GeometryShader> myGeometryShader;
		ComPtr<ID3D11PixelShader> myPixelShader;
		ComPtr<ID3D11InputLayout> myInputLayout;


		std::vector<Vertex> myOriginalVertex;
		std::vector<BlendShapeData> Blendshapes;
		Ref<Material> MaterialData;

		UINT myNumberOfVertices;
		UINT myNumberOfIndices;
		UINT myMaterialIndex;
		UINT myStride;
		UINT myOffset;
		UINT myPrimitiveTopology;

		std::string myMeshName;
	};

	struct BoxSphereBounds
	{
		std::vector<float> BoxExtents;
		std::vector<float> Center;
		float Radius;
	} BoxBounds;

private:

	int myMaterialSize = 0;
	std::wstring myPath;
	std::vector<MeshData> myMeshData = {};
	
	Skeleton mySkeleton;
	

public:


	void Update() override;


	void EditorUpdate();

	void Init(const MeshData& aMeshData, const std::wstring& aPath, Skeleton aSkeleton);
	void Init(const MeshData& aMeshData, const std::wstring& aPath);


	FORCEINLINE Skeleton* GetSkeleton()										{ return &mySkeleton; }
	FORCEINLINE const Skeleton* GetSkeleton() const							{ return &mySkeleton; }
	FORCEINLINE bool HasSkeleton() const									{ return mySkeleton.GetRoot(); }
	FORCEINLINE size_t GetNumMeshes() const									{ return myMeshData.size(); }
	FORCEINLINE const MeshData& GetMeshData(unsigned int anIndex = 0) const { return myMeshData[anIndex]; }
	FORCEINLINE MeshData& GetMeshData(unsigned int anIndex = 0)				{ return myMeshData[anIndex]; }
	FORCEINLINE const std::wstring& GetName() const							{ return myPath; }*/
};


