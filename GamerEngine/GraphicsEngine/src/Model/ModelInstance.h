#pragma once
#include <Model/Model.h>

#include "Time.hpp"
#include <Math/mathTypes.hpp>
#include <Model/Material.h>

#include "Render/LineRenderer.h"


class ModelInstance : public SceneObject
{
	std::shared_ptr<Model> myModel;
	int myModelMeshes;
	std::array<CommonUtilities::Matrix4x4<float>,MAX_MODEL_BONES> myBoneTransform{};
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

	void SetMaterial(Material* aMaterial) const { myModel->SetMaterial(aMaterial); }

	FORCEINLINE Material* GetMaterial() const { return myModel->GetMaterial(); }
	FORCEINLINE std::shared_ptr<AnimationStatus> GetAnimationState() { return myAnimState; }
	FORCEINLINE Skeleton* GetSkeleton() { return myModel->GetSkeleton(); }
	FORCEINLINE const Skeleton* GetSkeleton() const { return myModel->GetSkeleton(); }
	FORCEINLINE Model::MeshData const& GetMeshData(unsigned int anIndex) const { return myModel->GetMeshData(anIndex); }
	FORCEINLINE size_t GetNumMeshes() const { return myModel->GetNumMeshes(); }
	FORCEINLINE std::array<CommonUtilities::Matrix4x4<float>, MAX_MODEL_BONES> GetBoneTransform() { return myBoneTransform; }
};

inline void ModelInstance::Update()
{


	// TODO : TEST REMOVE LATE!!!!
	if(myModel)
	{
		if (myModel->test.BoxExtents.size() > 0)
		{
			float boxX = (myModel->test.BoxExtents[0] + myModel->test.Center[0]) * GetTransform().GetScale().x;
			float boxY = (myModel->test.BoxExtents[1] + myModel->test.Center[1]) * GetTransform().GetScale().y;
			float boxZ = (myModel->test.BoxExtents[2] + myModel->test.Center[2]) * GetTransform().GetScale().z;

			Vector3f bounds = { boxX, boxY, boxZ };
			Vector3f position = GetTransform().GetPosition();
			position.y += boxY * 0.5f;

			LineRenderer::DrawCube(position, bounds, GetTransform().GetRotation());
		}
	}


	SceneObject::Update();
	//myModel->Update();
	auto anAnimState = GetAnimationState();

	if (GetSkeleton()->GetRoot() && anAnimState->myCurrentAnimation != nullptr)
	{

		//myModel->Update();
		anAnimState->myCurrentTime += Time::GetDeltaTime();
		anAnimState->myCurrentFrame = static_cast<int>(anAnimState->myCurrentTime * anAnimState->myCurrentAnimation->FramesPerSecond);
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
	int length = static_cast<int>(anAnimState->myCurrentAnimation->Length);
	int frame = (anAnimState->myCurrentFrame < length - 1 ? anAnimState->myCurrentFrame + 1 : 0);
	Matrix4x4f lowFrame = anAnimState->myCurrentAnimation->Frames[anAnimState->myCurrentFrame].LocalTransforms[someBoneInd];
	Matrix4x4f highFrame = anAnimState->myCurrentAnimation->Frames[frame].LocalTransforms[someBoneInd];
	Matrix4x4f lerpFrame = Matrix4x4f::Slerp(lowFrame, highFrame, anAnimState->myInterFrameFraction);
	Matrix4x4f transposedFrame = (aParent * Matrix4x4f::Transpose(lerpFrame));
	myBoneTransform[someBoneInd] = transposedFrame * GetSkeleton()->Bones[someBoneInd].BindPoseInverse;
	
	for(int i = 0; i < GetSkeleton()->Bones[someBoneInd].Children.size(); i++)
	{
		UpdateAnimationHierarchy(anAnimState, GetSkeleton()->Bones[someBoneInd].Children[i], transposedFrame);
	}
}
