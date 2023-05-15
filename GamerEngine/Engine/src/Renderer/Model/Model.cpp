#include "GraphicsEngine.pch.h"
#include "Model.h"

<<<<<<< Updated upstream
void Model::EditorUpdate()
{
}

void Model::Init(const MeshData& aMeshData, const std::wstring& aPath, Skeleton aSkeleton)
{
	mySkeleton = std::move(aSkeleton);
	myMeshData.push_back(aMeshData);
	myPath = aPath;
}

void Model::Init(const MeshData& aMeshData, const std::wstring& aPath)
{
	myMeshData.push_back(aMeshData);
	myPath = aPath;
}

int Model::GetMaterialSize()
{
	return static_cast<int>(myMaterial.size());
}

void Model::SetMaterialSize(int aSize)
{
	if(aSize <= 0)
	{
		return;
	}

	myMaterial.resize(aSize);
	myMaterialSize = static_cast<int>(myMaterial.size());
}

void Model::PushMaterial(const Material aMaterial)
{
	myMaterialSize++;
	myMaterial.push_back(aMaterial);
=======
#include "Vertex.h"
#include "Renderer/Framework/DX11.h"

void Model::Update()
{
	SceneObject::Update();

	for(auto& meshData : myMeshData)
	{
		if(meshData.Blendshapes.empty())
		{
			return;
		}

		int i = 0;
		for(auto& blendShape : meshData.Blendshapes)
		{
			if(blendShape.WeightPercentOld != blendShape.WeightPercent)
			{
				D3D11_MAPPED_SUBRESOURCE mappedResource;
				HRESULT hr = DX11::Context->Map(meshData.myVertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
				if(SUCCEEDED(hr))
				{
					// Get a pointer to the data
					Vertex* pData = reinterpret_cast<Vertex*>(mappedResource.pData);

					std::vector<Vertex> Copy;


					

					for(auto& blendShape2 : meshData.Blendshapes)
					{
						for(int BlendPosIndex = 0; BlendPosIndex < blendShape2.BlendShapePosition.size(); BlendPosIndex++)
						{
							Vertex newData = meshData.myOriginalVertex[BlendPosIndex];
							newData.Position.x += blendShape2.WeightPercent * blendShape2.BlendShapePosition[BlendPosIndex].x;
							newData.Position.y += blendShape2.WeightPercent * blendShape2.BlendShapePosition[BlendPosIndex].y;
							newData.Position.z += blendShape2.WeightPercent * blendShape2.BlendShapePosition[BlendPosIndex].z;
							newData.Position.w += blendShape2.WeightPercent * blendShape2.BlendShapePosition[BlendPosIndex].w;
							Copy.push_back(newData);
						}
					}

					memcpy(pData, &Copy[0], sizeof(Vertex) * Copy.size());

					DX11::Context->Unmap(meshData.myVertexBuffer.Get(), 0);
				}
			}
			blendShape.WeightPercentOld = blendShape.WeightPercent;
			i++;
		}
	}
>>>>>>> Stashed changes
}
