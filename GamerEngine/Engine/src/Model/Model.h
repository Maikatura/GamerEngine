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

struct Animation
{
	struct Frame
	{
		std::vector<Matrix4x4f> LocalTransforms;
	};

	std::vector<Frame> Frames;
	unsigned int Length = 0;
	float Duration = 0.0f;
	float FramesPerSecond = 0.0f;
	std::wstring Name;
};


struct Bone
{
	Matrix4x4f BindPoseInverse;
	int ParentIdx = -1;
	std::vector<unsigned> Children;
};

struct Skeleton
{
	

	std::vector<Bone> Bones;
	std::unordered_map<std::string, size_t> BoneNameToIndex;
	std::vector<std::string> BoneNames;
	std::unordered_map<std::wstring, Animation> Animations;
	std::string Name;

	FORCEINLINE const Bone* GetRoot() const
	{
		if(!Bones.empty())
		{
			return (Bones).data();
		}
		return nullptr;
	}

};

struct AnimationStatus
{
	enum State
	{
		Playing,
		Paused,
		Done
	};
	State myState = Playing;
	float myInterFrameFraction = 0.0f;
	float myFraction = 0.0f;
	float myCurrentTime = 0.0f;
	int myCurrentFrame = 0;
	Animation* myCurrentAnimation = nullptr;
	Matrix4x4f myBoneTransforms[MAX_MODEL_BONES]{};
};

class Model : public SceneObject
{
public:

	struct BlendShapeData
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
	std::vector<Ref<Material>> myMaterial;
	std::vector<MeshData> myMeshData = {};
	
	Skeleton mySkeleton;
	

public:


	void Update() override;


	void EditorUpdate();

	void Init(const MeshData& aMeshData, const std::wstring& aPath, Skeleton aSkeleton);
	void Init(const MeshData& aMeshData, const std::wstring& aPath);

	int GetMaterialSize();
	void SetMaterialSize(int aSize);
	void PushMaterial(const Ref<Material> aMaterial);

	FORCEINLINE std::vector<Ref<Material>>& GetMaterial()		{ return myMaterial; }
	FORCEINLINE Skeleton* GetSkeleton()										{ return &mySkeleton; }
	FORCEINLINE const Skeleton* GetSkeleton() const							{ return &mySkeleton; }
	FORCEINLINE bool HasSkeleton() const									{ return mySkeleton.GetRoot(); }
	FORCEINLINE size_t GetNumMeshes() const									{ return myMeshData.size(); }
	FORCEINLINE const MeshData& GetMeshData(unsigned int anIndex = 0) const { return myMeshData[anIndex]; }
	FORCEINLINE MeshData& GetMeshData(unsigned int anIndex = 0)				{ return myMeshData[anIndex]; }
	FORCEINLINE const std::wstring& GetName() const							{ return myPath; }
};


