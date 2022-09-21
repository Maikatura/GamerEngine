#pragma once
#include <Model/Model.h>

#include "Time.hpp"
#include <Math/mathTypes.hpp>
#include <Model/Material.h>



class ModelInstance : public SceneObject
{
	std::shared_ptr<Model> myModel;
	int myModelMeshes;
	std::array<CommonUtilities::Matrix4x4<float>,128> myBoneTransform{};
	std::shared_ptr<AnimationStatus> myAnimState;

public:
	ModelInstance() = default;

	void Init(std::shared_ptr<Model> aModel) 
	{
		myModel = aModel;
		myAnimState = std::make_shared<AnimationStatus>();
	}

	std::shared_ptr<Model> GetModel() 
	{
		return myModel;
	}

	void PlayAnimation(std::wstring aAnimationPath)
	{
		myAnimState->myCurrentAnimation = &myModel->GetSkeleton()->Animations[aAnimationPath];
		myAnimState->myCurrentAnimation->Frames = myModel->GetSkeleton()->Animations[aAnimationPath].Frames;
	}

	virtual void Update() override;
	void UpdateAnimationHierarchy(AnimationStatus* anAnimState, int someBoneInd, CommonUtilities::Matrix4x4<float>& aParent);

	FORCEINLINE Material* GetMaterial() const
	{
		return myModel->GetMaterial();
	}
	FORCEINLINE std::shared_ptr<AnimationStatus> GetAnimationState() { return myAnimState; }
	FORCEINLINE Skeleton* GetSkeleton() { return myModel->GetSkeleton(); }
	FORCEINLINE const Skeleton* GetSkeleton() const { return myModel->GetSkeleton(); }
	FORCEINLINE Model::MeshData const& GetMeshData(unsigned int anIndex) const { return myModel->GetMeshData(anIndex); }
	FORCEINLINE size_t GetNumMeshes() const { return myModel->GetNumMeshes(); }
	FORCEINLINE std::array<CommonUtilities::Matrix4x4<float>, 128> GetBoneTransform() { return myBoneTransform; }
};

inline void ModelInstance::Update()
{
	SceneObject::Update();
	//myModel->Update();

	if (GetSkeleton()->GetRoot())
	{

		//myModel->Update();
		auto anAnimState = GetAnimationState();
		anAnimState->myCurrentTime += Time::GetDeltaTime();
		anAnimState->myCurrentFrame = (int)(anAnimState->myCurrentTime * anAnimState->myCurrentAnimation->FramesPerSecond);
		anAnimState->myFraction = anAnimState->myCurrentTime / anAnimState->myCurrentAnimation->Duration;
		anAnimState->myInterFrameFraction = ((0) + ((myAnimState->myCurrentTime) - ((float)myAnimState->myCurrentFrame / myAnimState->myCurrentAnimation->FramesPerSecond)) * ((1) - (0)) / (((float)(myAnimState->myCurrentFrame + 1) / myAnimState->myCurrentAnimation->FramesPerSecond) - ((float)myAnimState->myCurrentFrame / myAnimState->myCurrentAnimation->FramesPerSecond)));

		if(anAnimState->myFraction >= 1)
		{
			anAnimState->myCurrentTime = 0;
			anAnimState->myCurrentFrame = 0;
			anAnimState->myFraction = 0;
		}

		myBoneTransform[0] = CommonUtilities::Matrix4x4<float>();
		UpdateAnimationHierarchy(anAnimState.get(), 0, myBoneTransform[0]);
	}
}

inline void ModelInstance::UpdateAnimationHierarchy(AnimationStatus* anAnimState, int someBoneInd, CommonUtilities::Matrix4x4<float>& aParent)
{
	int frame = (anAnimState->myCurrentFrame < anAnimState->myCurrentAnimation->Length - 1 ? anAnimState->myCurrentFrame + 1 : 0);
	Matrix4x4f lowFrame = anAnimState->myCurrentAnimation->Frames[anAnimState->myCurrentFrame].LocalTransforms[static_cast<float>(someBoneInd)];
	Matrix4x4f highFrame = anAnimState->myCurrentAnimation->Frames[frame].LocalTransforms[static_cast<float>(someBoneInd)];
	Matrix4x4f lerpFrame = Matrix4x4f::Slerp(lowFrame, highFrame, anAnimState->myInterFrameFraction);
	Matrix4x4f transposedFrame = (aParent * Matrix4x4f::Transpose(lerpFrame));
	myBoneTransform[someBoneInd] = transposedFrame * GetSkeleton()->Bones[someBoneInd].BindPoseInverse;
	
	for(int i = 0; i < GetSkeleton()->Bones[someBoneInd].Children.size(); i++)
	{
		UpdateAnimationHierarchy(anAnimState, GetSkeleton()->Bones[someBoneInd].Children[i], transposedFrame);
	}
}
