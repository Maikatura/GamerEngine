#pragma once
#include <string>
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
	};

	struct BoxSphereBounds
	{
		std::vector<float> BoxExtents;
		std::vector<float> Center;
		float Radius;
	} BoxBounds;

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

	void SetMaterial(Material* aMaterial)									{ myMaterial = *aMaterial; }
	FORCEINLINE Material* GetMaterial()										{ return &myMaterial; }
	FORCEINLINE Skeleton* GetSkeleton()										{ return &mySkeleton; }
	FORCEINLINE const Skeleton* GetSkeleton() const							{ return &mySkeleton; }
	FORCEINLINE bool HasSkeleton() const									{ return mySkeleton.GetRoot(); }
	FORCEINLINE size_t GetNumMeshes() const									{ return myMeshData.size(); }
	FORCEINLINE const MeshData& GetMeshData(unsigned int anIndex = 0) const { return myMeshData[anIndex]; }
	FORCEINLINE const std::wstring& GetName() const							{ return myPath; }
};


