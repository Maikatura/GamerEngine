#pragma once
#include <Math/mathTypes.hpp>
#include <Math/AABB3D.hpp>
#include "SceneObject.h"
#include "Material.h"
#include "EngineSettings/Settings.h"
#include "Vertex.h"
#include "Core/Rendering/PixelShader.h"
#include "Core/Rendering/VertexShader.h"

class TransformComponent;


namespace GamerEngine
{
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
		bool ShouldLoop = true;
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
		std::unordered_map<size_t, std::string> IndexToBoneName;
		std::vector<std::string> BoneNames;
		std::unordered_map<std::wstring, Animation> Animations;
		std::string Name;

		FORCEINLINE const Bone* GetRoot() const
		{
			if (!Bones.empty())
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
		struct RenderedInstanceData
		{
			uint32_t EntityID;
			Matrix4x4f World;
		};

		struct MeshData
		{
			ComPtr<ID3D11Buffer> myVertexBuffer;
			ComPtr<ID3D11Buffer> myIndexBuffer;
			Ref<VertexShader> myVertexShader;
			ComPtr<ID3D11GeometryShader> myGeometryShader;
			Ref<PixelShader> myPixelShader;



			std::vector<Vertex> myOriginalVertex;
			Material MaterialData;

			UINT myNumberOfVertices;
			UINT myNumberOfIndices;
			UINT myMaterialIndex;
			UINT myStride;
			UINT myOffset;
			UINT myPrimitiveTopology;

			std::string myMeshName;
		};

	private:




		std::vector<MeshData> myMeshData = {};
		Skeleton mySkeleton;

		ComPtr<ID3D11Buffer> myInstanceBuffer;

		std::array<CommonUtilities::Matrix4x4<float>, MAX_MODEL_BONES> myBoneTransform{};
		AnimationStatus myAnimState;
		std::vector<RenderedInstanceData> myRenderedInstances;



		int myModelMeshes;

		bool myFirstTimeInit = true;
		bool myDoneRendering = true;
		bool myHasBeenRendered = false;
		bool myIsLoaded = false;
		std::wstring myPath;

		CommonUtilities::AABB3D<float> myBoxBounds;


	public:

		void SetSkeleton(Skeleton aSkeleton);
		void Init(const MeshData& aMeshData, const std::wstring& aPath);

		void AddRenderedInstance(uint32_t aEntityID, Matrix4x4f aTransform);
		bool UpdateInstanceBuffer();
		bool UseDeferred();

		bool IsLoaded() { return myIsLoaded; }

		const std::wstring& GetName() { return myPath; }

		CommonUtilities::AABB3D<float>& GetBoxBounds()
		{
			return myBoxBounds;
		}

		void SetBoxBounds(CommonUtilities::AABB3D<float> aBounds)
		{
			myBoxBounds = aBounds;
		}

		FORCEINLINE bool HasRenderedInstance() const
		{
			return (static_cast<int>(myRenderedInstances.size()) > 1) && !HasSkeleton();
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

		FORCEINLINE bool HasSkeleton() const { return mySkeleton.GetRoot(); }


		bool HasBeenRendered();
		void ClearInstanceData();
		void SetHasBeenRenderer(bool aValue);
		void EditorUpdate();

		Model() = default;


		void PlayAnimation(std::wstring aAnimationPath);
		virtual void Update();


		void UpdateAnimationHierarchy(AnimationStatus* anAnimState, unsigned int someBoneInd, CommonUtilities::Matrix4x4<float> aParent);

		FORCEINLINE AnimationStatus* GetAnimationState() { return &myAnimState; }
		FORCEINLINE Skeleton* GetSkeleton() { return &mySkeleton; }
		FORCEINLINE const Skeleton* GetSkeleton() const { return &mySkeleton; }
		MeshData& GetMeshData(unsigned int anIndex) { return myMeshData[anIndex]; }
		MeshData const& GetMeshData(unsigned int anIndex) const { return myMeshData[anIndex]; }
		FORCEINLINE size_t GetNumMeshes() const { return myMeshData.size(); }
		FORCEINLINE std::array<CommonUtilities::Matrix4x4<float>, MAX_MODEL_BONES> GetBoneTransform() { return myBoneTransform; }
	};
}



