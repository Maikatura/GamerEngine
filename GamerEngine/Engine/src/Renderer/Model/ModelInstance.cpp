#include "GraphicsEngine.pch.h"
#include "ModelInstance.h"

#include "Renderer/Framework/DX11.h"
#include "Components/Components.hpp"

void ModelInstance::AddRenderedInstance(TransformComponent* aTransform)
{
	RenderedInstanceData data = {aTransform};
	myRenderedInstances.push_back(data);
}

bool ModelInstance::UpdateInstanceBuffer()
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

	result = DX11::Device->CreateBuffer(&vertexBufferDesc, &vertexSubResourceData, myInstanceBuffer.GetAddressOf());
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

bool ModelInstance::RenderWithDeferred()
{
	if (myModel->HasSkeleton())
	{
		return true;
	}

	return false;
}

bool ModelInstance::HasBeenRendered()
{
	return myHasBeenRendered;
}

void ModelInstance::ClearInstanceData()
{
	myFirstTimeInit = true;
	myRenderedInstances.clear();
}

void ModelInstance::SetHasBeenRenderer(bool aValue)
{
	myHasBeenRendered = aValue;
}

void ModelInstance::Init(std::shared_ptr<Model> aModel)
{
	myTransform = std::make_shared<Transform>();
	if (!aModel->HasSkeleton())
	{
		if (myFirstTimeInit)
		{
			myModel = aModel;
			myFirstTimeInit = false;
		}
	}
	else
	{
		myModel = aModel;
		myAnimState = std::make_shared<AnimationStatus>();

	}


}

std::shared_ptr<Model> ModelInstance::GetModel()
{
	if (!myModel)
	{
		return nullptr;
	}

	return myModel;
}

void ModelInstance::PlayAnimation(std::wstring aAnimationPath)
{
	myAnimState->myCurrentAnimation = &myModel->GetSkeleton()->Animations[aAnimationPath];
	myAnimState->myCurrentAnimation->Frames = myModel->GetSkeleton()->Animations[aAnimationPath].Frames;
}

void ModelInstance::Update()
{
	// TODO : TEST REMOVE LATE!!!!
	/*if(myModel)
	{
		if(myModel->test.BoxExtents.size() > 0)
		{
			float boxX = (myModel->test.BoxExtents[0] + myModel->test.Center[0]) * GetTransform().GetScale().x;
			float boxY = (myModel->test.BoxExtents[1] + myModel->test.Center[1]) * GetTransform().GetScale().y;
			float boxZ = (myModel->test.BoxExtents[2] + myModel->test.Center[2]) * GetTransform().GetScale().z;

			Vector3f bounds = { boxX, boxY, boxZ };
			Vector3f position = GetTransform().GetPosition();
			position.y += boxY * 0.5f;


			LineRenderer::DrawCube(position, bounds, GetTransform().GetRotation());
		}
	}*/

	

	SceneObject::Update();
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

void ModelInstance::EditorUpdate()
{
	if(myModel)
	{
		myModel->Update();
	}
}

void ModelInstance::UpdateAnimationHierarchy(AnimationStatus* anAnimState, int someBoneInd, CommonUtilities::Matrix4x4<float>& aParent)
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
