#include "GraphicsEngine.pch.h"
#include "ModelAssetHandler.h"
#include <Model/Vertex.h>
#include "Framework/DX11.h"
#include <fstream>
#include <vector>
#include <StringCast.h>
#include <Importer/FBXImporter.h>


bool ModelAssetHandler::InitUnitCube()
{
	HRESULT result = S_FALSE;

	std::vector<Vertex> mdlVertices =
	{
		{
			{-0.5f,-0.5f,-0.5f, 1.0f },
			{ {0.0f, 0.0f, 0.0f, 1.0f, }}

		},
		{
			{-0.5f,-0.5f, 0.5f, 1.0f},
			{{ 0.0f,0.0f,1.0f,1.0f }}
		},
		{
			{-0.5f, 0.5f,-0.5f,1.0f},
			{{0.0f,1.0f,0.0f,1.0f }}
		},
		{
			{-0.5f, 0.5f, 0.5f,1.0f},
			{ {1.0f,0.0f,1.0f,1.0f}}
		},
		{
			{0.5f,-0.5f,-0.5f,1.0f},
			{{0.0f,1.0f,1.0f,1.0f}}
		},
		{
			{0.5f,-0.5f, 0.5f,1.0f},
			{{1.0f,1.0f,0.0f,1.0f}}
		},
		{
			{ 0.5f, 0.5f,-0.5f,1.0f},
			{{1.0f,1.0f,1.0f,1.0f }}
		},
		{
			{ 0.5f, 0.5f, 0.5f,1.0f},
			{{0.0f,0.0f,0.0f,1.0f}}
		}
	};

	D3D11_BUFFER_DESC vertexBufferDesc = {};
	vertexBufferDesc.ByteWidth = static_cast<UINT>(mdlVertices.size()) * static_cast<UINT>(sizeof(Vertex));
	vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA vertexSubResourceData = {};
	vertexSubResourceData.pSysMem = &mdlVertices[0];

	ID3D11Buffer* vertexBuffer;
	result = DX11::Device->CreateBuffer(&vertexBufferDesc, &vertexSubResourceData, &vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	std::vector<unsigned int> mdlIndices =
	{
		0,2,1, // -x
		1,2,3,

		4,5,6, // +x
		5,7,6,

		0,1,5, // -y
		0,5,4,

		2,6,7, // +y
		2,7,3,

		0,4,6, // -z
		0,6,2,

		1,3,7, // +z
		1,7,5,
	};

	D3D11_BUFFER_DESC indexBufferDesc = {};
	indexBufferDesc.ByteWidth = static_cast<UINT>(mdlIndices.size()) * static_cast<UINT>(sizeof(unsigned int));
	indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA indexSubresourceData = {};
	indexSubresourceData.pSysMem = &mdlIndices[0];

	ID3D11Buffer* indexBuffer;
	result = DX11::Device->CreateBuffer(&indexBufferDesc, &indexSubresourceData, &indexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	auto layout = CreateLayout();

	std::ifstream vsFile;
	vsFile.open("Shaders\\DefaultVS.cso", std::ios::binary);
	std::string vsData = { std::istreambuf_iterator<char>(vsFile), std::istreambuf_iterator<char>() };
	ID3D11VertexShader* vertexShader;
	result = DX11::Device->CreateVertexShader(vsData.data(), vsData.size(), nullptr, &vertexShader);
	if (FAILED(result))
	{
		return false;
	}
	vsFile.close();

	std::ifstream psFile;
	psFile.open("Shaders\\DefaultPS.cso", std::ios::binary);
	std::string psData = { std::istreambuf_iterator<char>(psFile), std::istreambuf_iterator<char>() };
	ID3D11PixelShader* pixelShader;
	result = DX11::Device->CreatePixelShader(psData.data(), psData.size(), nullptr, &pixelShader);
	if (FAILED(result))
	{
		return false;
	}
	psFile.close();

	ID3D11InputLayout* inputLayout;
	result = DX11::Device->CreateInputLayout(layout.data(), sizeof(layout) / sizeof(D3D11_INPUT_ELEMENT_DESC), vsData.data(), vsData.size(), &inputLayout);
	if (FAILED(result))
	{
		return false;
	}

	Model::MeshData modelData = {};
	modelData.myNumberOfVertices = static_cast<UINT>(mdlVertices.size());
	modelData.myNumberOfIndices = static_cast<UINT>(mdlIndices.size());
	modelData.myStride = sizeof(Vertex);
	modelData.myOffset = 0;
	modelData.myVertexBuffer = vertexBuffer;
	modelData.myIndexBuffer = indexBuffer;
	modelData.myVertexShader = vertexShader;
	modelData.myPixelShader = pixelShader;
	modelData.myPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	modelData.myInputLayout = inputLayout;

	std::shared_ptr<Model> mdl = std::make_shared<Model>();
	mdl->Init(modelData, L"Cube");
	myModelRegistry.insert({ L"Cube", mdl });


	return true;
}

bool ModelAssetHandler::InitUnitSphere()
{
	

	return true;
}

void ModelAssetHandler::Clear()
{
	myModelRegistry.clear();
}

bool ModelAssetHandler::Initialize()
{
	if (!InitUnitCube())
	{
		return false;
	}

	if (!InitUnitSphere())
	{
		return false;
	}

	return true;
}

void ModelAssetHandler::UnloadModel(const std::wstring& aFilePath)
{
	myModelRegistry.erase(aFilePath);

}

std::array<D3D11_INPUT_ELEMENT_DESC, InputLayoutSize> ModelAssetHandler::CreateLayout()
{
	std::array<D3D11_INPUT_ELEMENT_DESC, InputLayoutSize> layout =
	{
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXCOORD", 1, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXCOORD", 2, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXCOORD", 3, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{ "BONEIDS", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0 , D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{ "BONEWEIGHTS", 0 , DXGI_FORMAT_R32G32B32A32_FLOAT, 0 , D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{ "TANGENT", 0 , DXGI_FORMAT_R32G32B32A32_FLOAT, 0 , D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "BINORMAL", 0 , DXGI_FORMAT_R32G32B32A32_FLOAT, 0 , D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0 , DXGI_FORMAT_R32G32B32A32_FLOAT, 0 , D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		}
	};

	return layout;
}

bool ModelAssetHandler::LoadModel(const std::wstring& aFilePath)
{
	const std::string ansiFileName = Helpers::string_cast<std::string>(aFilePath);

	HRESULT result = S_FALSE;

	TGA::FBXModel tgaModel;
	if (TGA::FBXImporter::LoadModel(ansiFileName, tgaModel))
	{
		std::shared_ptr<Model> mdl = std::make_shared<Model>();
	
		
		

		for (size_t i = 0; i < tgaModel.Meshes.size(); i++)
		{
			std::vector<Vertex> mdlVertices;

			std::vector<unsigned int> mdlIndices;
			TGA::FBXModel::FBXMesh& mesh = tgaModel.Meshes[i];
			mdlVertices.resize(mesh.Vertices.size());

			for(size_t v = 0; v < mesh.Vertices.size(); v++)
			{
				
				mdlVertices[v].Position = {
					mesh.Vertices[v].Position[0],
					mesh.Vertices[v].Position[1],
					mesh.Vertices[v].Position[2],
					1.0f
				};

				for (int vCol = 0; vCol < 4; vCol++)
				{
					mdlVertices[v].VertexColors[vCol] = {
						mesh.Vertices[v].VertexColors[vCol][0],
						mesh.Vertices[v].VertexColors[vCol][1],
						mesh.Vertices[v].VertexColors[vCol][2],
						mesh.Vertices[v].VertexColors[vCol][3],
					};
				}
				for (int uvCh = 0; uvCh < 4; uvCh++)
				{
					mdlVertices[v].UVs[uvCh].x = mesh.Vertices[v].UVs[uvCh][0];
					mdlVertices[v].UVs[uvCh].y = mesh.Vertices[v].UVs[uvCh][1];
				}

				mdlVertices[v].myBoneIDs = {
					mesh.Vertices[v].BoneIDs[0],
					mesh.Vertices[v].BoneIDs[1],
					mesh.Vertices[v].BoneIDs[2],
					mesh.Vertices[v].BoneIDs[3]
				};

				mdlVertices[v].myBoneWeights = {
					mesh.Vertices[v].BoneWeights[0],
					mesh.Vertices[v].BoneWeights[1],
					mesh.Vertices[v].BoneWeights[2],
					mesh.Vertices[v].BoneWeights[3]
				};

				mdlVertices[v].Tangent = {
					mesh.Vertices[v].Tangent[0],
					mesh.Vertices[v].Tangent[1],
					mesh.Vertices[v].Tangent[2],
				};

				mdlVertices[v].Binormal = {
					mesh.Vertices[v].Binormal[0],
					mesh.Vertices[v].Binormal[1],
					mesh.Vertices[v].Binormal[2],
				};

				mdlVertices[v].Normal = {
					mesh.Vertices[v].Normal[0],
					mesh.Vertices[v].Normal[1],
					mesh.Vertices[v].Normal[2],
				};

			}

			mdlIndices = mesh.Indices;

			Skeleton mdlSkeleton;
			const bool hasSkeleton = tgaModel.Skeleton.GetRoot();
			if(hasSkeleton)
			{
				for(size_t i = 0; i < tgaModel.Skeleton.Bones.size(); i++)
				{
					mdlSkeleton.Bones.push_back(*(Skeleton::Bone*)&tgaModel.Skeleton.Bones[i]);
					mdlSkeleton.Bones[i].BindPoseInverse = *(CommonUtilities::Matrix4x4<float>*)&tgaModel.Skeleton.Bones[i].BindPoseInverse;
					mdlSkeleton.Bones[i].Children = tgaModel.Skeleton.Bones[i].Children;
					mdlSkeleton.Bones[i].Name = tgaModel.Skeleton.Bones[i].Name;
					mdlSkeleton.BoneNames.push_back(tgaModel.Skeleton.Bones[i].Name);
					mdlSkeleton.BoneNameToIndex[tgaModel.Skeleton.Bones[i].Name] = i;
					mdlSkeleton.Bones[i].Parent = tgaModel.Skeleton.Bones[i].Parent;
				}
			}

			D3D11_BUFFER_DESC vertexBufferDesc = {};
			vertexBufferDesc.ByteWidth = static_cast<UINT>(mdlVertices.size()) * static_cast<UINT>(sizeof(Vertex));
			vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
			vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

			D3D11_SUBRESOURCE_DATA vertexSubResourceData = {};
			vertexSubResourceData.pSysMem = &mdlVertices[0];
			vertexSubResourceData.SysMemPitch = 0;
			vertexSubResourceData.SysMemSlicePitch = 0;

			ID3D11Buffer* vertexBuffer;
			result = DX11::Device->CreateBuffer(&vertexBufferDesc, &vertexSubResourceData, &vertexBuffer);
			if(FAILED(result))
			{
				return false;
			}

			D3D11_BUFFER_DESC indexBufferDesc = {};
			indexBufferDesc.ByteWidth = static_cast<UINT>(mdlIndices.size()) * static_cast<UINT>(sizeof(unsigned int));
			indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
			indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

			D3D11_SUBRESOURCE_DATA indexSubresourceData = {};
			indexSubresourceData.pSysMem = &mdlIndices[0];
			indexSubresourceData.SysMemPitch = 0;
			indexSubresourceData.SysMemSlicePitch = 0;

			ID3D11Buffer* indexBuffer;
			result = DX11::Device->CreateBuffer(&indexBufferDesc, &indexSubresourceData, &indexBuffer);
			if(FAILED(result))
			{
				return false;
			}

		
			auto layout = CreateLayout();

			std::ifstream vsFile;
			vsFile.open("Shaders/DefaultVS.cso", std::ios::binary);
			std::string vsData = { std::istreambuf_iterator<char>(vsFile), std::istreambuf_iterator<char>() };
			ID3D11VertexShader* vertexShader;
			result = DX11::Device->CreateVertexShader(vsData.data(), vsData.size(), nullptr, &vertexShader);
			if(FAILED(result))
			{
				return false;
			}
			vsFile.close();

			std::ifstream psFile;
			psFile.open("Shaders/DefaultPS.cso", std::ios::binary);
			std::string psData = { std::istreambuf_iterator<char>(psFile), std::istreambuf_iterator<char>() };
			ID3D11PixelShader* pixelShader;
			result = DX11::Device->CreatePixelShader(psData.data(), psData.size(), nullptr, &pixelShader);
			if(FAILED(result))
			{
				return false;
			}
			psFile.close();

			ID3D11InputLayout* inputLayout;
			result = DX11::Device->CreateInputLayout(layout.data(), sizeof(layout) / sizeof(D3D11_INPUT_ELEMENT_DESC), vsData.data(), vsData.size(), &inputLayout);
			if(FAILED(result))
			{
				return false;
			}

			Model::MeshData modelData = {};
			modelData.myNumberOfVertices = static_cast<UINT>(mdlVertices.size());
			modelData.myNumberOfIndices = static_cast<UINT>(mdlIndices.size());
			modelData.myStride = sizeof(Vertex);
			modelData.myOffset = 0;
			modelData.myVertexBuffer = vertexBuffer;
			modelData.myIndexBuffer = indexBuffer;
			modelData.myVertexShader = vertexShader;
			modelData.myPixelShader = pixelShader;
			modelData.myPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
			modelData.myInputLayout = inputLayout;

			auto test = Helpers::CleanModelName(aFilePath);
			std::wstring texture = L"Assets\\Models\\T_" + test + L"_C.dds";

			TextureAssetHandler::LoadTexture(texture);
			mdl->GetMaterial()->SetAlbedoTexture(TextureAssetHandler::GetTexture(texture));

			if(hasSkeleton)
			{
				mdl->Init(modelData, aFilePath, mdlSkeleton);
			}
			else
			{
				mdl->Init(modelData, aFilePath);
			}

			myModelRegistry.insert({ aFilePath, mdl });
		}

		return true;
	}

	return false;
}

bool ModelAssetHandler::LoadAnimation(const std::wstring& aModelName, const std::wstring& someFilePath) 
{
	const std::string ansiFileName = Helpers::string_cast<std::string>(someFilePath);
	std::shared_ptr<ModelInstance> model = GetModelInstance(aModelName);

	TGA::FBXAnimation tgaAnimation;
	if (TGA::FBXImporter::LoadAnimation(ansiFileName, myModelRegistry[aModelName]->GetSkeleton()->BoneNames, tgaAnimation))
	{
		Animation animOut;
		animOut.Duration = static_cast<float>(tgaAnimation.Duration);
		animOut.FramesPerSecond = tgaAnimation.FramesPerSecond;
		animOut.Length = tgaAnimation.Length;
		animOut.Name = Helpers::string_cast<std::wstring>(tgaAnimation.Name);
		for(int i = 0; i < static_cast<int>(tgaAnimation.Length); i++)
		{
			animOut.Frames.push_back(Animation::Frame{ *reinterpret_cast<std::vector<CommonUtilities::Matrix4x4<float>>*>(&tgaAnimation.Frames[i].LocalTransforms) });
		}
		
		model->GetSkeleton()->Animations.insert({ someFilePath, animOut });
	}


	return true;
}

std::shared_ptr<Model> ModelAssetHandler::GetModel(const std::wstring& aFilePath) 
{
	auto model = myModelRegistry.find(aFilePath);

	if (model->second == nullptr)
	{
		LoadModel(aFilePath);
		model = myModelRegistry.find(aFilePath);
	}

	return model->second;
}

std::shared_ptr<ModelInstance> ModelAssetHandler::GetModelInstance(const std::wstring& aFilePath) 
{
	auto modelInstance = std::make_shared<ModelInstance>();
	auto model = myModelRegistry.find(aFilePath);


	if(model == myModelRegistry.end())
	{
		LoadModel(aFilePath);
		model = myModelRegistry.find(aFilePath);
	}

	modelInstance->Init(model->second);
	return modelInstance;
}
