#include "GraphicsEngine.pch.h"
#include "Model.h"

#include "Core/Framework/DX11.h"
#include "Components/Components.hpp"

void GamerEngine::Model::Init(const MeshData& aMeshData, const std::wstring& aPath, Skeleton aSkeleton)
{
	mySkeleton = aSkeleton;
	Init(aMeshData, aPath);
}

void  GamerEngine::Model::Init(const MeshData& aMeshData, const std::wstring& aPath)
{
	myMeshData.push_back(aMeshData);
	myPath = aPath;
	
	if (!HasSkeleton())
	{
		if (myFirstTimeInit)
		{
			myFirstTimeInit = false;
		}
	}
	else
	{
		myAnimState = MakeRef<AnimationStatus>();
	}
}

void GamerEngine::Model::AddRenderedInstance(TransformComponent* aTransform)
{
	RenderedInstanceData data = {aTransform};
	myRenderedInstances.push_back(data);
}

bool GamerEngine::Model::UpdateInstanceBuffer()
{
	HRESULT result = E_FAIL;

	D3D11_BUFFER_DESC vertexBufferDesc{};

	vertexBufferDesc.ByteWidth = static_cast<UINT>(sizeof(RenderedInstanceData) * myRenderedInstances.size());
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vertexSubResourceData{};
	vertexSubResourceData.pSysMem = myRenderedInstances.data();

	result = DX11::Get().Get().GetDevice()->CreateBuffer(&vertexBufferDesc, &vertexSubResourceData, myInstanceBuffer.GetAddressOf());
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

bool GamerEngine::Model::UseDeferred()
{
	return true;
	if (HasSkeleton())
	{
		return false;
	}

	return false;
}

bool GamerEngine::Model::HasBeenRendered()
{
	return myHasBeenRendered;
}

void GamerEngine::Model::ClearInstanceData()
{
	myFirstTimeInit = true;
	myRenderedInstances.clear();
}

void GamerEngine::Model::SetHasBeenRenderer(bool aValue)
{
	myHasBeenRendered = aValue;
}

void GamerEngine::Model::PlayAnimation(std::wstring aAnimationPath)
{
	myAnimState->myCurrentAnimation = &GetSkeleton()->Animations[aAnimationPath];
	myAnimState->myCurrentAnimation->Frames = GetSkeleton()->Animations[aAnimationPath].Frames;
}

void GamerEngine::Model::Update()
{
	auto anAnimState = GetAnimationState();

	if(GetSkeleton()->GetRoot() && anAnimState->myCurrentAnimation != nullptr)
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

void GamerEngine::Model::EditorUpdate()
{
	
}

void GamerEngine::Model::UpdateAnimationHierarchy(AnimationStatus* anAnimState, int someBoneInd, CommonUtilities::Matrix4x4<float>& aParent)
{
	int length = static_cast<int>(anAnimState->myCurrentAnimation->Length);

	if (length <= 0)
	{
		return;
	}

	int frame = (anAnimState->myCurrentFrame < length - 1 ? anAnimState->myCurrentFrame + 1 : 0);
	Matrix4x4f lowFrame = anAnimState->myCurrentAnimation->Frames[anAnimState->myCurrentFrame].LocalTransforms[someBoneInd];
	Matrix4x4f highFrame = anAnimState->myCurrentAnimation->Frames[frame].LocalTransforms[someBoneInd];
	Matrix4x4f lerpFrame = Matrix4x4f::Slerp(lowFrame, highFrame, anAnimState->myInterFrameFraction);
	Matrix4x4f transposedFrame = (aParent * Matrix4x4f::Transpose(lerpFrame));
	Matrix4x4f transformFrame = transposedFrame * GetSkeleton()->Bones[someBoneInd].BindPoseInverse;
	myBoneTransform[someBoneInd] = transformFrame;

	
	for(int i = 0; i < GetSkeleton()->Bones[someBoneInd].Children.size(); i++)
	{
		UpdateAnimationHierarchy(anAnimState, GetSkeleton()->Bones[someBoneInd].Children[i], transposedFrame);
	}
}
