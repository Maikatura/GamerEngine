#pragma once
#include <Renderer/Model/Model.h>

#include "Time.hpp"
#include <Math/mathTypes.hpp>
#include <Renderer/Model/Material.h>

class TransformComponent;

class ModelInstance : public SceneObject
{
	bool myFirstTimeInit = true;
	bool myDoneRendering = true;
	bool myHasBeenRendered = false;
	bool myIsLoaded = false;

public:
	struct RenderedInstanceData
	{
		TransformComponent* World;
	};



private:

	
	std::shared_ptr<Model> myModel;
	int myModelMeshes;
	std::array<CommonUtilities::Matrix4x4<float>,MAX_MODEL_BONES> myBoneTransform{};
	std::shared_ptr<AnimationStatus> myAnimState;



	ComPtr<ID3D11Buffer> myInstanceBuffer;
	std::vector<RenderedInstanceData> myRenderedInstances;

public:

	void AddRenderedInstance(TransformComponent* aTransform);
	bool UpdateInstanceBuffer();
	bool RenderWithDeferred();

	bool IsLoaded() { return myIsLoaded; }

	FORCEINLINE bool HasRenderedInstance() const
	{
		return !myRenderedInstances.empty();
	}

	FORCEINLINE unsigned GetNumberOfInstances() const
	{
		return static_cast<unsigned>(myRenderedInstances.size());
	}

	FORCEINLINE ComPtr<ID3D11Buffer> GetInstanceBuffer() const
	{
		return myInstanceBuffer;
	}

	FORCEINLINE std::vector<RenderedInstanceData>& GetTransformVector()
	{
		return myRenderedInstances;
	}

	bool HasBeenRendered();
	void ClearInstanceData();
	void SetHasBeenRenderer(bool aValue);

	ModelInstance() = default;

	void Init(std::shared_ptr<Model> aModel);
	std::shared_ptr<Model> GetModel();


	void PlayAnimation(std::wstring aAnimationPath);
	virtual void Update() override;
	void UpdateAnimationHierarchy(AnimationStatus* anAnimState, int someBoneInd, CommonUtilities::Matrix4x4<float>& aParent);

	void PushMaterial(Material aMaterial) const { myModel->PushMaterial(aMaterial); }

	FORCEINLINE std::vector<Material>& GetMaterial() const { return myModel->GetMaterial(); }
	FORCEINLINE int GetMaterialSize() { return myModel->GetMaterialSize(); }
	FORCEINLINE std::shared_ptr<AnimationStatus> GetAnimationState() { return myAnimState; }
	FORCEINLINE Skeleton* GetSkeleton() { return myModel->GetSkeleton(); }
	FORCEINLINE const Skeleton* GetSkeleton() const { return myModel->GetSkeleton(); }
	FORCEINLINE Model::MeshData const& GetMeshData(unsigned int anIndex) const { return myModel->GetMeshData(anIndex); }
	FORCEINLINE size_t GetNumMeshes() const { return myModel->GetNumMeshes(); }
	FORCEINLINE std::array<CommonUtilities::Matrix4x4<float>, MAX_MODEL_BONES> GetBoneTransform() { return myBoneTransform; }
};



