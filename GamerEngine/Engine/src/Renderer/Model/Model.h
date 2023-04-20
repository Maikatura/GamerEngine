#pragma once
#include <string>
#include <utility>
#include <vector>
#include <Renderer/Model/SceneObject.h>
#include <unordered_map>
#include <Windows.h>
#include <d3d11.h>
#include <Math/MathTypes.hpp>
#include "Material.h"
#include <Renderer/EngineSettings/Settings.h>

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


struct Skeleton
{
	std::string Name;

	struct Bone
	{
		Matrix4x4f BindPoseInverse;
		int ParentIdx = -1;
		std::string Name;
		std::vector<unsigned> Children;
	};

	std::vector<Bone> Bones;
	std::unordered_map<std::string, size_t> BoneNameToIndex;
	std::vector<std::string> BoneNames;

	std::unordered_map<std::wstring, Animation> Animations;

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
		std::string MeshName;
		std::string BlendShapeName;
		std::vector<unsigned int> AffectedIndexes;     // all Vertices affected by blendshape
		std::vector<Vector4f> BlendShapePosition; // Where the affected Vertex should go to when the blendshape is at 100%
		float WeightPercent;
	};

	struct MeshData 
	{
		ComPtr<ID3D11Buffer> myVertexBuffer;
		ComPtr<ID3D11Buffer> myIndexBuffer;
		ComPtr<ID3D11VertexShader> myVertexShader;
		ComPtr<ID3D11GeometryShader> myGeometryShader;
		ComPtr<ID3D11PixelShader> myPixelShader;
		ComPtr<ID3D11InputLayout> myInputLayout;

		UINT myPrimitiveTopology;
		UINT myNumberOfVertices;
		UINT myNumberOfIndices;
		UINT myStride;
		UINT myOffset;

		UINT myMaterialIndex;
		std::string myMeshName;
		std::vector<BlendShapeData> Blendshapes;
	};

	struct BoxSphereBounds
	{
		std::vector<float> BoxExtents;
		std::vector<float> Center;
		float Radius;
	} BoxBounds;

	



private:




	int myMaterialSize = 0;
	Skeleton mySkeleton;
	std::vector<Material> myMaterial;
	std::vector<MeshData> myMeshData = {};
	
	std::wstring myPath;
	

public:
	void Init(const MeshData& aMeshData, const std::wstring& aPath, Skeleton aSkeleton)
	{
		mySkeleton = std::move(aSkeleton);
		myMeshData.push_back(aMeshData);
		myPath = aPath;
	}
	void Init(const MeshData& aMeshData, const std::wstring& aPath)
	{
		myMeshData.push_back(aMeshData);
		myPath = aPath;
	}

	int GetMaterialSize()
	{
		return static_cast<int>(myMaterial.size());
	}
	void SetMaterialSize(int aSize)
	{
		if (aSize <= 0)
		{
			return;
		}

		myMaterial.resize(aSize);
		myMaterialSize = static_cast<int>(myMaterial.size());
	}

	void PushMaterial(const Material aMaterial)
	{
		myMaterialSize++;
		myMaterial.push_back(aMaterial);
	}
	FORCEINLINE std::vector<Material>& GetMaterial()						{ return myMaterial; }
	FORCEINLINE Skeleton* GetSkeleton()										{ return &mySkeleton; }
	FORCEINLINE const Skeleton* GetSkeleton() const							{ return &mySkeleton; }
	FORCEINLINE bool HasSkeleton() const									{ return mySkeleton.GetRoot(); }
	FORCEINLINE size_t GetNumMeshes() const									{ return myMeshData.size(); }
	FORCEINLINE const MeshData& GetMeshData(unsigned int anIndex = 0) const { return myMeshData[anIndex]; }
	FORCEINLINE MeshData& GetMeshData(unsigned int anIndex = 0)				{ return myMeshData[anIndex]; }
	FORCEINLINE const std::wstring& GetName() const							{ return myPath; }
};


