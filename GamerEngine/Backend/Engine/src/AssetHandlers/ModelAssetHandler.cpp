#include "GraphicsEngine.pch.h"
#include "ModelAssetHandler.h"
#include <Core/Model/Vertex.h>
#include "Core/Framework/DX11.h"
#include <fstream>
#include <vector>
#include <Utilites/StringCast.h>
#include <Importer.h>

#include "Scene/SceneManager.h"
#include <ofbx.h>

#include "ShaderHandler.h"

ModelAssetHandler& ModelAssetHandler::Get()
{
	static ModelAssetHandler instance(1);
	return instance;
}

bool ModelAssetHandler::InitUnitCube()
{
	HRESULT result = S_FALSE;

	//// TODO : Fix so that the cube is not inverted

	//std::vector<Vertex> mdlVertices =
	//{
	//	{
	//				{0.5f, 0.5f, 0.5f, 1},
	//{0.0f, 0.0f, 0.0f},
	//				{{1, 1, 1, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}},
	//				{{1, 0}, {1, 1}, {1, 1}, {1, 1}},
	//				{0, 0, 0, 0}, {0, 0, 0, 0},
	//				{0, 1, 0}, {0, 0, -1}, {1, 0, 0}
	//			},
	//			{
	//				{0.5f, 0.5f, -0.5f, 1},
	//				{0.0f, 0.0f, 0.0f},
	//				{{1, 1, 0, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}},
	//				{{1, 1}, {1, 1}, {1, 1}, {1, 1}},
	//				{0, 0, 0, 0}, {0, 0, 0, 0},
	//				{0, 1, 0}, {0, 0, -1}, {1, 0, 0}
	//			},
	//			{
	//				{0.5f, -0.5f, 0.5f, 1},
	//				{0.0f, 0.0f, 0.0f},
	//				{{1, 0, 1, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}},
	//				{{0, 0}, {1, 1}, {1, 1}, {1, 1}},
	//				{0, 0, 0, 0}, {0, 0, 0, 0},
	//				{0, 1, 0}, {0, 0, -1}, {1, 0, 0}
	//			},
	//			{
	//				{0.5f, -0.5f, -0.5f, 1},
	//	{0.0f, 0.0f, 0.0f},
	//				{{1, 0, 0, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}},
	//				{{0, 1}, {1, 1}, {1, 1}, {1, 1}},
	//				{0, 0, 0, 0}, {0, 0, 0, 0},
	//				{0, 1, 0}, {0, 0, -1}, {1, 0, 0}
	//			},
	//			{
	//				{-0.5f, 0.5f, 0.5f, 1},
	//				{0.0f, 0.0f, 0.0f},
	//				{{0, 1, 1, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}},
	//				{{1, 1}, {1, 1}, {1, 1}, {1, 1}},
	//				{0, 0, 0, 0}, {0, 0, 0, 0},
	//				{0, -1, 0}, {0, 0, 1}, {-1, 0, 0}
	//			},
	//			{
	//				{-0.5f, 0.5f, -0.5f, 1},
	//{0.0f, 0.0f, 0.0f},
	//				{{0, 1, 0, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}},
	//				{{1, 0}, {1, 1}, {1, 1}, {1, 1}},
	//				{0, 0, 0, 0}, {0, 0, 0, 0},
	//				{0, -1, 0}, {0, 0, 1}, {-1, 0, 0}
	//			},
	//			{
	//				{-0.5f, -0.5f, 0.5f, 1},
	//				{0.0f, 0.0f, 0.0f},
	//				{{0, 0, 1, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}},
	//				{{0, 1}, {1, 1}, {1, 1}, {1, 1}},
	//				{0, 0, 0, 0}, {0, 0, 0, 0},
	//				{0, -1, 0}, {0, 0, 1}, {-1, 0, 0}
	//			},
	//			{
	//				{-0.5f, -0.5f, -0.5f, 1},
	//				{0.0f, 0.0f, 0.0f},
	//				{{0, 0, 0, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}},
	//				{{0, 0}, {1, 1}, {1, 1}, {1, 1}},
	//				{0, 0, 0, 0}, {0, 0, 0, 0},
	//				{0, -1, 0}, {0, 0, 1}, {-1, 0, 0}
	//			},

	//			{
	//				{0.5f, 0.5f, 0.5f, 1},
	//	{0.0f, 0.0f, 0.0f},
	//				{{1, 1, 1, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}},
	//				{{0, 0}, {1, 1}, {1, 1}, {1, 1}},
	//				{0, 0, 0, 0}, {0, 0, 0, 0},
	//				{1, 0, 0}, {0, 0, -1}, {0, 1, 0}
	//			},
	//			{
	//				{0.5f, 0.5f, -0.5f, 1},
	//{0.0f, 0.0f, 0.0f},
	//				{{1, 1, 0, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}},
	//				{{1, 0}, {1, 1}, {1, 1}, {1, 1}},
	//				{0, 0, 0, 0}, {0, 0, 0, 0},
	//				{1, 0, 0}, {0, 0, -1}, {0, 1, 0}
	//			},
	//			{
	//				{0.5f, -0.5f, 0.5f, 1},
	//	{0.0f, 0.0f, 0.0f},
	//				{{1, 0, 1, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}},
	//				{{1, 0}, {1, 1}, {1, 1}, {1, 1}},
	//				{0, 0, 0, 0}, {0, 0, 0, 0},
	//				{-1, 0, 0}, {0, 0, 1}, {0, -1, 0}
	//			},
	//			{
	//				{0.5f, -0.5f, -0.5f, 1},
	//			{0.0f, 0.0f, 0.0f},
	//				{{1, 0, 0, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}},
	//				{{0, 0}, {1, 1}, {1, 1}, {1, 1}},
	//				{0, 0, 0, 0}, {0, 0, 0, 0},
	//				{-1, 0, 0}, {0, 0, 1}, {0, -1, 0}
	//			},
	//			{
	//				{-0.5f, 0.5f, 0.5f, 1},
	//				{0.0f, 0.0f, 0.0f},
	//				{{0, 1, 1, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}},
	//				{{0, 1}, {1, 1}, {1, 1}, {1, 1}},
	//				{0, 0, 0, 0}, {0, 0, 0, 0},
	//				{1, 0, 0}, {0, 0, -1}, {0, 1, 0}
	//			},
	//			{
	//				{-0.5f, 0.5f, -0.5f, 1},
	//				{0.0f, 0.0f, 0.0f},
	//				{{0, 1, 0, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}},
	//				{{1, 1}, {1, 1}, {1, 1}, {1, 1}},
	//				{0, 0, 0, 0}, {0, 0, 0, 0},
	//				{1, 0, 0}, {0, 0, -1}, {0, 1, 0}
	//			},
	//			{
	//				{-0.5f, -0.5f, 0.5f, 1},
	//				{0.0f, 0.0f, 0.0f},
	//				{{0, 0, 1, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}},
	//				{{1, 1}, {1, 1}, {1, 1}, {1, 1}},
	//				{0, 0, 0, 0}, {0, 0, 0, 0},
	//				{-1, 0, 0}, {0, 0, 1}, {0, -1, 0}
	//			},
	//			{
	//				{-0.5f, -0.5f, -0.5f, 1},
	//				{0.0f, 0.0f, 0.0f},
	//				{{0, 0, 0, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}},
	//				{{0, 1}, {1, 1}, {1, 1}, {1, 1}},
	//				{0, 0, 0, 0}, {0, 0, 0, 0},
	//				{-1, 0, 0}, {0, 0, 1}, {0, -1, 0}
	//			},

	//			{
	//				{0.5f, 0.5f, 0.5f, 1},
	//				{0.0f, 0.0f, 0.0f},
	//				{{1, 1, 1, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}},
	//				{{0, 0}, {1, 1}, {1, 1}, {1, 1}},
	//				{0, 0, 0, 0}, {0, 0, 0, 0},
	//				{1, 0, 0}, {0, -1, 0}, {0, 0, 1}
	//			},
	//			{
	//				{0.5f, 0.5f, -0.5f, 1},
	//				{0.0f, 0.0f, 0.0f},
	//				{{1, 1, 0, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}},
	//				{{1, 0}, {1, 1}, {1, 1}, {1, 1}},
	//				{0, 0, 0, 0}, {0, 0, 0, 0},
	//				{-1, 0, 0}, {0, 1, 0}, {0, 0, -1}
	//			},
	//			{
	//				{0.5f, -0.5f, 0.5f, 1},
	//			{0.0f, 0.0f, 0.0f},
	//				{{1, 0, 1, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}},
	//				{{0, 1}, {1, 1}, {1, 1}, {1, 1}},
	//				{0, 0, 0, 0}, {0, 0, 0, 0},
	//				{1, 0, 0}, {0, -1, 0}, {0, 0, 1}
	//			},
	//			{
	//				{0.5f, -0.5f, -0.5f, 1},
	//		{0.0f, 0.0f, 0.0f},
	//				{{1, 0, 0, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}},
	//				{{1, 1}, {1, 1}, {1, 1}, {1, 1}},
	//				{0, 0, 0, 0}, {0, 0, 0, 0},
	//				{-1, 0, 0}, {0, 1, 0}, {0, 0, -1}
	//			},
	//			{
	//				{-0.5f, 0.5f, 0.5f, 1},
	//			{0.0f, 0.0f, 0.0f},
	//				{{0, 1, 1, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}},
	//				{{1, 0}, {1, 1}, {1, 1}, {1, 1}},
	//				{0, 0, 0, 0}, {0, 0, 0, 0},
	//				{1, 0, 0}, {0, -1, 0}, {0, 0, 1}
	//			},
	//			{
	//				{-0.5f, 0.5f, -0.5f, 1},
	//			{0.0f, 0.0f, 0.0f},
	//				{{0, 1, 0, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}},
	//				{{0, 0}, {1, 1}, {1, 1}, {1, 1}},
	//				{0, 0, 0, 0}, {0, 0, 0, 0},
	//				{-1, 0, 0}, {0, 1, 0}, {0, 0, -1}
	//			},
	//			{
	//				{-0.5f, -0.5f, 0.5f, 1},
	//				{0.0f, 0.0f, 0.0f},
	//				{{0, 0, 1, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}},
	//				{{1, 1}, {1, 1}, {1, 1}, {1, 1}},
	//				{0, 0, 0, 0}, {0, 0, 0, 0},
	//				{1, 0, 0}, {0, -1, 0}, {0, 0, 1}
	//			},
	//			{
	//				{-0.5f, -0.5f, -0.5f, 1},
	//				{0.0f, 0.0f, 0.0f},
	//				{{0, 0, 0, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}},
	//				{{0, 1}, {1, 1}, {1, 1}, {1, 1}},
	//				{0, 0, 0, 0}, {0, 0, 0, 0},
	//				{-1, 0, 0}, {0, 1, 0}, {0, 0, -1}
	//			}
	//};


	//D3D11_BUFFER_DESC vertexBufferDesc = {};
	//vertexBufferDesc.ByteWidth = static_cast<UINT>(mdlVertices.size()) * static_cast<UINT>(sizeof(Vertex));
	//vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	//vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	//D3D11_SUBRESOURCE_DATA vertexSubResourceData = {};
	//vertexSubResourceData.pSysMem = &mdlVertices[0];

	//ID3D11Buffer* vertexBuffer;
	//result = DX11::Get().Device->CreateBuffer(&vertexBufferDesc, &vertexSubResourceData, &vertexBuffer);
	//if(FAILED(result))
	//{
	//	return false;
	//}

	//std::vector<unsigned int> mdlIndices =
	//{
	//	2,1,0,
	//	1,2,3,

	//	// Left
	//	6,4,5,
	//	7,6,5,

	//	// Top
	//	8,9,12,
	//	12,9,13,

	//	// Bottom
	//	15,11,10,
	//	14,15,10,

	//	// Front
	//	16,20,22,
	//	22,18,16,

	//	// Back
	//	23,21,17,
	//	17,19,23
	//};

	//D3D11_BUFFER_DESC indexBufferDesc = {};
	//indexBufferDesc.ByteWidth = static_cast<UINT>(mdlIndices.size()) * static_cast<UINT>(sizeof(unsigned int));
	//indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	//indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	//D3D11_SUBRESOURCE_DATA indexSubresourceData = {};
	//indexSubresourceData.pSysMem = &mdlIndices[0];

	//ID3D11Buffer* indexBuffer;
	//result = DX11::Get().GetDevice()->CreateBuffer(&indexBufferDesc, &indexSubresourceData, &indexBuffer);
	//if(FAILED(result))
	//{
	//	return false;
	//}

	//auto layout = CreateLayout();

	//std::ifstream vsFile;
	//vsFile.open("Shaders\\Default_VS.cso", std::ios::binary);
	//std::string vsData = { std::istreambuf_iterator<char>(vsFile), std::istreambuf_iterator<char>() };
	//ID3D11VertexShader* vertexShader;
	//result = DX11::Get().GetDevice()->CreateVertexShader(vsData.data(), vsData.size(), nullptr, &vertexShader);
	//if(FAILED(result))
	//{
	//	return false;
	//}
	//vsFile.close();

	//ComPtr<ID3D11PixelShader> pixelShader = TextureAssetHandler::GetPixelShader("Shaders\\Default_PS.cso");

	//ID3D11InputLayout* inputLayout;
	//result = DX11::Get().GetDevice()->CreateInputLayout(layout.data(), sizeof(layout) / sizeof(D3D11_INPUT_ELEMENT_DESC), vsData.data(), vsData.size(), &inputLayout);
	//if(FAILED(result))
	//{
	//	return false;
	//}

	//Model::MeshData modelData = {};
	//modelData.myNumberOfVertices = static_cast<UINT>(mdlVertices.size());
	//modelData.myNumberOfIndices = static_cast<UINT>(mdlIndices.size());
	//modelData.myStride = sizeof(Vertex);
	//modelData.myOffset = 0;
	//modelData.myVertexBuffer = vertexBuffer;
	//modelData.myIndexBuffer = indexBuffer;
	//modelData.myVertexShader = vertexShader;
	//modelData.myPixelShader = pixelShader;
	//modelData.myPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	//modelData.myInputLayout = inputLayout;
	//modelData.myVertexShader = vertexShader;
	//modelData.myPixelShader = pixelShader;
	//modelData.myGeometryShader = nullptr;

	//Ref<Model> mdl = MakeRef<Model>();

	//std::wstring albedoTexture = L"Resources\\Textures\\T_Default_C.dds";
	//std::wstring normalTexture = L"Resources\\Textures\\T_Default_N.dds";
	//std::wstring materialTexture = L"Resources\\Textures\\T_Default_M.dds";



	//Ref<Material> mat = MakeRef<Material>();
	//mat->Init(L"Cube");
	//mat->SetAlbedoTexture(TextureAssetHandler::GetTexture(albedoTexture));
	//mat->SetNormalTexture(TextureAssetHandler::GetTexture(normalTexture));
	//mat->SetMaterialTexture(TextureAssetHandler::GetTexture(materialTexture));
	//mdl->PushMaterial(mat);

	//mdl->Init(modelData, L"Cube");

	//Ref<Model> mdlInstance = MakeRef<Model>();
	//mdlInstance->Init(mdl);

	//myModelRegistry.insert({ L"Cube", mdlInstance });


	return true;
}

bool ModelAssetHandler::InitUnitSphere()
{


	return true;
}

ofbx::IScene* ModelAssetHandler::LoadModelScene(const std::string& aPath)
{

	FILE* fp;
	fopen_s(&fp, aPath.c_str(), "rb");

	if (!fp)
	{
		return nullptr;
	}

	fseek(fp, 0, SEEK_END);
	long file_size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	auto* content = new ofbx::u8[file_size];
	fread(content, 1, file_size, fp);


	ofbx::LoadFlags flags =
		ofbx::LoadFlags::TRIANGULATE |
		//		ofbx::LoadFlags::IGNORE_MODELS |
		ofbx::LoadFlags::IGNORE_BLEND_SHAPES |
		ofbx::LoadFlags::IGNORE_CAMERAS |
		ofbx::LoadFlags::IGNORE_LIGHTS |
		//		ofbx::LoadFlags::IGNORE_TEXTURES |
		ofbx::LoadFlags::IGNORE_SKIN |
		ofbx::LoadFlags::IGNORE_BONES |
		ofbx::LoadFlags::IGNORE_PIVOTS |
		//		ofbx::LoadFlags::IGNORE_MATERIALS |
		ofbx::LoadFlags::IGNORE_POSES |
		ofbx::LoadFlags::IGNORE_VIDEOS |
		ofbx::LoadFlags::IGNORE_LIMBS |
		//		ofbx::LoadFlags::IGNORE_MESHES |
		ofbx::LoadFlags::IGNORE_ANIMATIONS;

	ofbx::IScene* myScene = ofbx::load((ofbx::u8*)content, file_size, (ofbx::u16)flags);


	return myScene;
}

void ModelAssetHandler::Clear()
{
	for (auto item : myModelRegistry)
	{
		if (item == nullptr)
		{
			continue;
		}
		item->ClearInstanceData();

		/*if (item.second == nullptr)
		{
			continue;
		}

		item.second->ClearInstanceData();
		std::cout << "Name: " << item.second->Name << " Cleared Data" << "\n";*/
	}


}

bool ModelAssetHandler::Init()
{

	if (SceneManager::Get().IsHeadless())
	{
		return true;
	}

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
	aFilePath;
	//myModelRegistry.erase(aFilePath);
}

void ModelAssetHandler::ResetRenderedModels()
{
	for (auto item : myModelRegistry)
	{
		if (item)
		{
			item->SetHasBeenRenderer(false);
		}

		/*if (item.second)
		{
			item.second->SetHasBeenRenderer(false);
		}*/
	}
}

bool ModelAssetHandler::LoadModelNewTesting(const std::wstring& aFilePath)
{
	// // Convert the file path to a string
	// std::string filePath = std::string(aFilePath.begin(), aFilePath.end());
	//
	// // Open the file in binary mode
	// std::ifstream file(filePath, std::ios::binary | std::ios::ate);
	// if (!file)
	// {
	// 	std::cerr << "Failed to open file: " << filePath << std::endl;
	// 	return false;
	// }
	//
	// // Get the size of the file
	// std::streamsize size = file.tellg();
	// file.seekg(0, std::ios::beg);
	//
	// // Create a buffer and read the file into it
	// std::vector<char> buffer(size);
	// if (!file.read(buffer.data(), size))
	// {
	// 	std::cerr << "Failed to read file: " << filePath << std::endl;
	// 	return false;
	// }
	//
	// // Load the scene from the buffer
	// ofbx::IScene* scene = ofbx::load((ofbx::u8*)buffer.data(), static_cast<int>(size), ofbx::LoadFlags::TRIANGULATE);
	// if (!scene)
	// {
	// 	std::cerr << "Failed to load scene: " << filePath << std::endl;
	// 	return false;
	// }
	//
	// // Iterate over all meshes in the scene
	// for (int i = 0; i < scene->getMeshCount(); ++i)
	// {
	// 	const ofbx::Mesh* mesh = scene->getMesh(i);
	//
	// 	// Get the vertex data
	// 	const ofbx::Vec3* vertices = mesh->getGeometry()->getVertices();
	// 	int vertexCount = mesh->getGeometry()->getVertexCount();
	//
	// 	// Get the index data
	// 	const int* indices = mesh->getGeometry()->getFaceIndices();
	// 	int indexCount = mesh->getGeometry()->getIndexCount();
	//
	// 	// Create a vector to hold our vertices
	// 	std::vector<Vertex> vertexData(vertexCount);
	//
	// 	// Copy the vertex data into our vector
	// 	for (int j = 0; j < vertexCount; ++j)
	// 	{
	// 		vertexData[j].Position.x = static_cast<float>(vertices[j].x);
	// 		vertexData[j].Position.y = static_cast<float>(vertices[j].y);
	// 		vertexData[j].Position.z = static_cast<float>(vertices[j].z);
	// 	}
	//
	// 	// Create a vector to hold our indices
	// 	std::vector<int> indexData(indices, indices + indexCount);
	//
	// 	// Now you can create a mesh object using vertexData and indexData
	// 	// Mesh myMesh(vertexData, indexData);
	//
	// 	// If the mesh has a material, you can also extract the material properties
	// 	const ofbx::Material* material = mesh->getMaterial(indexCount);
	// 	if (material)
	// 	{
	// 		// Get the material properties, like the albedo color, specular color, etc.
	// 		// You can use these properties to create a material object for your mesh
	// 		ofbx::Color albedoColor = material->getDiffuseColor();
	// 		ofbx::Color specularColor = material->getSpecularColor();
	// 		// ...
	// 	}
	// }
	//
	// // Don't forget to destroy the scene when you're done with it
	// scene->destroy();

	return true;
}

bool ModelAssetHandler::LoadModelData(const std::wstring& aFilePath)
{
	if (SceneManager::Get().IsHeadless())
	{
		return true;
	}


#ifdef USE_NEW_IMPORTER
	return LoadModelNewTesting(aFilePath);
#else


	const std::string ansiFileName = Helpers::string_cast<std::string>(Helpers::ToLowerCase(aFilePath));

	HRESULT result = S_FALSE;

	std::vector<std::string> addedBlendShapes;


	TGA::FBX::Mesh tgaModel;
	TGA::FBX::Importer::InitImporter();
	if (TGA::FBX::Importer::LoadModel(aFilePath, tgaModel))
	{
		Ref<GamerEngine::Model> mdlInstance = MakeRef<GamerEngine::Model>();

		for (size_t i = 0; i < tgaModel.Elements.size(); i++)
		{
			std::vector<Vertex> mdlVertices;

			std::vector<unsigned int> mdlIndices;
			auto& mesh = tgaModel.Elements[i];
			mdlVertices.resize(mesh.Vertices.size());



			for (size_t v = 0; v < mesh.Vertices.size(); v++)
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
						mesh.Vertices[v].VertexColors[vCol][3]
					};
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

				for (int uvCh = 0; uvCh < 4; uvCh++)
				{
					mdlVertices[v].UVs[uvCh].x = std::clamp(mesh.Vertices[v].UVs[uvCh][0], 0.0f, 1.0f);
					mdlVertices[v].UVs[uvCh].y = std::clamp(mesh.Vertices[v].UVs[uvCh][1], 0.0f, 1.0f);

				}


				mdlVertices[v].Tangent = {
					mesh.Vertices[v].Tangent[0],
					mesh.Vertices[v].Tangent[1],
					mesh.Vertices[v].Tangent[2]
				};

				mdlVertices[v].Binormal = {
					mesh.Vertices[v].BiNormal[0],
					mesh.Vertices[v].BiNormal[1],
					mesh.Vertices[v].BiNormal[2]
				};

				mdlVertices[v].Normal = {
					mesh.Vertices[v].Normal[0],
					mesh.Vertices[v].Normal[1],
					mesh.Vertices[v].Normal[2]
				};


			}

			CommonUtilities::AABB3D<float> bounds;
			bounds.InitWithMinAndMax(
				{ tgaModel.BoxBounds.Min[0],
					tgaModel.BoxBounds.Min[1],
					tgaModel.BoxBounds.Min[2]
				},
				{
					tgaModel.BoxBounds.Max[0],
					tgaModel.BoxBounds.Max[1],
					tgaModel.BoxBounds.Max[2]
				});

			mdlInstance->SetBoxBounds(bounds);

			int size = static_cast<int>(mesh.Indices.size());
			mdlIndices.resize(size);
			memcpy_s(&mdlIndices[0], sizeof(unsigned) * size, &mesh.Indices[0], sizeof(unsigned) * size);


			/*for (int i = 0; i < mesh.Indices.size(); i++)
			{


				mdlIndices.push_back(mesh.Indices[i]);
			}*/


			

			D3D11_BUFFER_DESC vertexBufferDesc = {};
			vertexBufferDesc.ByteWidth = static_cast<UINT>(mdlVertices.size()) * static_cast<UINT>(sizeof(Vertex));
			vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
			//vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
			vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

			vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;


			D3D11_SUBRESOURCE_DATA vertexSubResourceData = {};
			vertexSubResourceData.pSysMem = &mdlVertices[0];
			vertexSubResourceData.SysMemPitch = 0;
			vertexSubResourceData.SysMemSlicePitch = 0;

			ID3D11Buffer* vertexBuffer;
			result = DX11::Get().GetDevice()->CreateBuffer(&vertexBufferDesc, &vertexSubResourceData, &vertexBuffer);
			if (FAILED(result))
			{
				return false;
			}

			D3D11_BUFFER_DESC indexBufferDesc = {};
			indexBufferDesc.ByteWidth = static_cast<UINT>(mdlIndices.size()) * static_cast<UINT>(sizeof(unsigned));
			indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
			indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

			D3D11_SUBRESOURCE_DATA indexSubresourceData = {};
			indexSubresourceData.pSysMem = &mdlIndices[0];
			indexSubresourceData.SysMemPitch = 0;
			indexSubresourceData.SysMemSlicePitch = 0;

			ID3D11Buffer* indexBuffer;
			result = DX11::Get().GetDevice()->CreateBuffer(&indexBufferDesc, &indexSubresourceData, &indexBuffer);
			if (FAILED(result))
			{
				return false;
			}
			
			GamerEngine::Model::MeshData modelData = {};
			modelData.myNumberOfVertices = static_cast<UINT>(mdlVertices.size());
			modelData.myNumberOfIndices = static_cast<UINT>(mdlIndices.size());
			modelData.myStride = sizeof(Vertex);
			modelData.myOffset = 0;
			modelData.myVertexBuffer = vertexBuffer;
			modelData.myIndexBuffer = indexBuffer;
			modelData.myVertexShader = ShaderHandler::Get().LoadVertexShader("Shaders\\Default_VS.cso");
			modelData.myPixelShader = ShaderHandler::Get().LoadPixelShader("Shaders\\Default_PS.cso");
			modelData.myPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
			modelData.myMeshName = mesh.MeshName;
			modelData.myMaterialIndex = mesh.MaterialIndex;
			modelData.myOriginalVertex = mdlVertices;

			auto fileName = Helpers::CleanModelName(aFilePath);
			std::wstring albedoTexture = L"Assets\\Textures\\T_" + fileName + L"_C.dds";
			std::wstring normalTexture = L"Assets\\Textures\\T_" + fileName + L"_N.dds";
			std::wstring materialTexture = L"Assets\\Textures\\T_" + fileName + L"_M.dds";

			auto albedo = TextureAssetHandler::GetTexture(albedoTexture);
			auto normal = TextureAssetHandler::GetTexture(normalTexture);
			auto material = TextureAssetHandler::GetTexture(materialTexture);

			if (normal->GetName() == albedo->GetName())
			{
				normal = TextureAssetHandler::GetTexture(L"resources\\Textures\\T_Default_N.dds");
			}

			if (material->GetName() == albedo->GetName())
			{
				material = TextureAssetHandler::GetTexture(L"resources\\Textures\\T_Default_M.dds");
			}

			modelData.MaterialData.Init(Helpers::string_cast<std::wstring>(mesh.MeshName));
			modelData.MaterialData.SetAlbedoTexture(albedo);
			modelData.MaterialData.SetNormalTexture(normal);
			modelData.MaterialData.SetMaterialTexture(material);



			/*for(size_t i = 0; i < mesh.Blendshapes.size(); i++)
			{
				Model::BlendShapeData data;
				data.Name = mesh.Blendshapes[i].Name;

				for(size_t x = 0; x < mesh.Blendshapes[i].Vertex.size(); x++)
				{
					data.BlendShapeVertex.push_back(*(Vertex*)&mesh.Blendshapes[i].Vertex[x]);
				}

				modelData.Blendshapes.push_back(data);
			}*/

			//D3D11_BUFFER_DESC blendShapeBufferDesc = {};
			//blendShapeBufferDesc.ByteWidth = static_cast<UINT>(modelData.Blendshapes.size()) * static_cast<UINT>(sizeof(Vertex));
			//blendShapeBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
			////blendShapeBuffer.Usage = D3D11_USAGE_DEFAULT;
			//blendShapeBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			//blendShapeBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;


			//D3D11_SUBRESOURCE_DATA blendShapeSubResourceData = {};
			//blendShapeSubResourceData.pSysMem = &mdlVertices[0];
			//blendShapeSubResourceData.SysMemPitch = 0;
			//blendShapeSubResourceData.SysMemSlicePitch = 0;

			//ID3D11Buffer* blendBuffer;
			//result = DX11::Get().Device->CreateBuffer(&blendShapeBufferDesc, &blendShapeSubResourceData, &blendBuffer);
			//if(FAILED(result))
			//{
			//	return false;
			//}

			//modelData.myBlendShapeBuffer = blendBuffer;

			mdlInstance->Init(modelData, Helpers::ToLowerCase(aFilePath));

			
		}

		GamerEngine::Skeleton mdlSkeleton;
		const bool hasSkeleton = tgaModel.Skeleton.GetRoot();
		if (hasSkeleton)
		{
			for (size_t i = 0; i < tgaModel.Skeleton.Bones.size(); i++)
			{
				mdlSkeleton.Bones.push_back(GamerEngine::Bone());
				mdlSkeleton.Bones[i].BindPoseInverse = Matrix4x4f(std::array<float, 16>{
					tgaModel.Skeleton.Bones[i].BindPoseInverse[0], tgaModel.Skeleton.Bones[i].BindPoseInverse[1], tgaModel.Skeleton.Bones[i].BindPoseInverse[2], tgaModel.Skeleton.Bones[i].BindPoseInverse[3],
					tgaModel.Skeleton.Bones[i].BindPoseInverse[4], tgaModel.Skeleton.Bones[i].BindPoseInverse[5], tgaModel.Skeleton.Bones[i].BindPoseInverse[6], tgaModel.Skeleton.Bones[i].BindPoseInverse[7],
					tgaModel.Skeleton.Bones[i].BindPoseInverse[8], tgaModel.Skeleton.Bones[i].BindPoseInverse[9], tgaModel.Skeleton.Bones[i].BindPoseInverse[10], tgaModel.Skeleton.Bones[i].BindPoseInverse[11],
					tgaModel.Skeleton.Bones[i].BindPoseInverse[12], tgaModel.Skeleton.Bones[i].BindPoseInverse[13], tgaModel.Skeleton.Bones[i].BindPoseInverse[14], tgaModel.Skeleton.Bones[i].BindPoseInverse[15]
				});
				mdlSkeleton.Bones[i].Children = tgaModel.Skeleton.Bones[i].Children;
				//mdlSkeleton.Bones[i].Name = tgaModel.Skeleton.Bones[i].Name;
				mdlSkeleton.Bones[i].ParentIdx = tgaModel.Skeleton.Bones[i].ParentIdx;
				mdlSkeleton.BoneNames.push_back(tgaModel.Skeleton.Bones[i].Name);
				mdlSkeleton.BoneNameToIndex[tgaModel.Skeleton.Bones[i].Name] = i;
				mdlSkeleton.IndexToBoneName[i] = tgaModel.Skeleton.Bones[i].Name;
			}
		}

		if (hasSkeleton)
		{
			mdlInstance->SetSkeleton(mdlSkeleton);
		}
		

		mdlInstance->Name = ansiFileName;
		myModelRegistry.push_back(mdlInstance);

		return true;
	}

	return false;
#endif
}

bool ModelAssetHandler::LoadAnimationData(const std::wstring& aModelName, const std::wstring& someFilePath)
{
	const std::string ansiFileName = Helpers::string_cast<std::string>(Helpers::ToLowerCase(someFilePath));
	Ref<GamerEngine::Model> model = GetModelInstance(Helpers::ToLowerCase(aModelName));

	TGA::FBX::Animation tgaAnimation;
	if (!TGA::FBX::Importer::LoadAnimation(Helpers::ToLowerCase(someFilePath), tgaAnimation))
	{
		return false;
	}

	GamerEngine::Animation animOut;
	animOut.Duration = static_cast<float>(tgaAnimation.Duration);
	animOut.FramesPerSecond = tgaAnimation.FramesPerSecond;
	animOut.Length = tgaAnimation.Length;
	animOut.Name = Helpers::string_cast<std::wstring>(tgaAnimation.Name);
	for (int i = 0; i < static_cast<int>(tgaAnimation.Length); i++)
	{


		GamerEngine::Animation::Frame frames;
		frames.LocalTransforms.resize(model->GetSkeleton()->Bones.size());

		//{ *(std::vector<Matrix4x4f>*)& tgaAnimation.Frames[i].LocalTransforms }
		for (auto bonePair : tgaAnimation.Frames[i].LocalTransforms)
		{
			auto bone = bonePair.first;
			auto index = model->GetSkeleton()->BoneNameToIndex[bone];

			Matrix4x4f anim = Matrix4x4f(std::array<float, 16>{
				tgaAnimation.Frames[i].LocalTransforms[bone][0], tgaAnimation.Frames[i].LocalTransforms[bone][1], tgaAnimation.Frames[i].LocalTransforms[bone][2], tgaAnimation.Frames[i].LocalTransforms[bone][3],
					tgaAnimation.Frames[i].LocalTransforms[bone][4], tgaAnimation.Frames[i].LocalTransforms[bone][5], tgaAnimation.Frames[i].LocalTransforms[bone][6], tgaAnimation.Frames[i].LocalTransforms[bone][7],
					tgaAnimation.Frames[i].LocalTransforms[bone][8], tgaAnimation.Frames[i].LocalTransforms[bone][9], tgaAnimation.Frames[i].LocalTransforms[bone][10], tgaAnimation.Frames[i].LocalTransforms[bone][11],
					tgaAnimation.Frames[i].LocalTransforms[bone][12], tgaAnimation.Frames[i].LocalTransforms[bone][13], tgaAnimation.Frames[i].LocalTransforms[bone][14], tgaAnimation.Frames[i].LocalTransforms[bone][15]
			});
			frames.LocalTransforms[index] = anim; // (*(Matrix4x4f*)&tgaAnimation.Frames[i].LocalTransforms);
			//frames.LocalTransforms.push_back(*(Matrix4x4f*)&tgaAnimation.Frames[i].LocalTransforms);
		}

		animOut.Frames.push_back(frames);
	}

	model->GetSkeleton()->Animations.insert({ Helpers::ToLowerCase(someFilePath), animOut });

	return true;
}

//Ref<Model> ModelAssetHandler::GetModel(const std::wstring& aFilePath)
//{
//	auto model = myModelRegistry.find(aFilePath);
//
//	if (model->second == nullptr)
//	{
//		LoadModelData(aFilePath);
//		model = myModelRegistry.find(aFilePath);
//	}
//
//	return model->second->GetModel();
//}

Ref<GamerEngine::Model> ModelAssetHandler::GetModelInstance(const std::wstring& aFilePath)
{

	if (!std::filesystem::exists(aFilePath))
	{
		return nullptr;
	}

	{
		/*std::scoped_lock<std::mutex> lock(myListMutex);
		if (LoadModelData(Helpers::ToLowerCase(aFilePath)))
		{
			return myModelRegistry.back();
		}*/
	}


	for (auto& item : myModelRegistry)
	{
		if (item->GetName() == Helpers::ToLowerCase(aFilePath))
		{
			return item;
		}
	}


	if (LoadModelData(Helpers::ToLowerCase(aFilePath)))
	{
		for (auto& item : myModelRegistry)
		{
			if (item->GetName() == Helpers::ToLowerCase(aFilePath))
			{
				return item;
			}
		}
	}

	if (myModelRegistry.empty())
	{
		auto returnValue = MakeRef<GamerEngine::Model>();
		return returnValue;
	}

	return nullptr;
}
