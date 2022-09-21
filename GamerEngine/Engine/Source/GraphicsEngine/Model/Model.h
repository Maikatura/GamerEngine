#pragma once
#include <wrl.h>
#include <string>
#include <vector>
#include <Model/SceneObject.h>
#include "Vertex.h"
#include <unordered_map>
#include <Windows.h>
#include <d3d11.h>
#include <Math/MathTypes.hpp>

#include "Material.h"

using namespace Microsoft::WRL;

struct Animation
{
	struct Frame
	{
		std::vector<Matrix4x4f> LocalTransforms;
	};

	std::vector<Frame> Frames;
	unsigned int Length;
	float Duration;
	float FramesPerSecond;
	std::wstring Name;
};


struct Skeleton
{
	std::string Name;

	struct Bone
	{
		Matrix4x4f BindPoseInverse;
		int Parent;
		std::vector<unsigned int> Children;
		std::string Name;
	};

	std::vector<Bone> Bones;
	std::unordered_map<std::string, size_t> BoneNameToIndex;
	std::vector<std::string> BoneNames;

	std::unordered_map<std::wstring, Animation> Animations;

	FORCEINLINE const Bone* GetRoot() const
	{
		if(!Bones.empty())
		{
			return &Bones[0];
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
	State myState;
	float myInterFrameFraction;
	float myFraction;
	float myCurrentTime;
	int myCurrentFrame;
	Animation* myCurrentAnimation;
	Matrix4x4f myBoneTransforms[128]{};
};

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

		Vector3f myMeshColor{ 0, 0, 0 };
		
	};

private:


	Skeleton mySkeleton;
	Material myMaterial;
	std::vector<MeshData> myMeshData = {};
	std::wstring myPath;
	

public:
	void Init(MeshData aMeshData, const std::wstring& aPath, Skeleton aSkeleton)
	{
		mySkeleton = aSkeleton;
		myMeshData.push_back(aMeshData);
		myPath = aPath;
	}

	void Init(MeshData aMeshData, const std::wstring& aPath) 
	{
		myMeshData.push_back(aMeshData);
		myPath = aPath;
	}

	FORCEINLINE Material* GetMaterial() { return &myMaterial; }
	FORCEINLINE Skeleton* GetSkeleton()										{ return &mySkeleton; }
	FORCEINLINE const Skeleton* GetSkeleton() const							{ return &mySkeleton; }
	FORCEINLINE size_t const& GetNumMeshes()								{ return myMeshData.size(); }
	FORCEINLINE MeshData const& GetMeshData(unsigned int anIndex = 0) const { return myMeshData[anIndex]; }
	FORCEINLINE std::wstring const& GetName() const							{ return myPath; }
};


