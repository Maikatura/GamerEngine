#include "GraphicsEngine.pch.h"
#include "Model.h"

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
}
