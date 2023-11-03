#include "GraphicsEngine.pch.h"
#include "ModelAssetHandler.h"
#include <Model/Vertex.h>
#include "Framework/DX11.h"
#include <fstream>
#include <vector>
#include <Utilites/StringCast.h>
#include <Importer.h>

#include "Scene/SceneManager.h"
#include <ofbx.h>

TGA::FBX::Importer importer;

std::array<D3D11_INPUT_ELEMENT_DESC, InputLayoutSize> ModelAssetHandler::CreateLayout()
{
	std::array<D3D11_INPUT_ELEMENT_DESC, InputLayoutSize> layout =
	{
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},

			{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},

			{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"COLOR", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"COLOR", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"COLOR", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},

			{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXCOORD", 1, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXCOORD", 2, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXCOORD", 3, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},

			{"BONEIDS", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"BONEWEIGHTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},

			// Instance Data
			{"WORLD", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1},
			{"WORLD", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1},
			{"WORLD", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1},
			{"WORLD", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1}

		}
	};

	return layout;
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

	//Ref<ModelInstance> mdlInstance = MakeRef<ModelInstance>();
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

ModelAssetHandler& ModelAssetHandler::Get()
{
	static ModelAssetHandler instance(4);

	return instance;
}

void ModelAssetHandler::Clear()
{
	for (auto item : myModelRegistry)
	{
		if (item.second == nullptr)
		{
			continue;
		}

		item.second->ClearInstanceData();
		std::cout << "Name: " << item.second->GetModel()->Name << " Cleared Data" << "\n";
	}


}

bool ModelAssetHandler::Initialize()
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

	importer.InitImporter();

	return true;
}

void ModelAssetHandler::UnloadModel(const std::wstring& aFilePath)
{
	myModelRegistry.erase(aFilePath);

}

void ModelAssetHandler::ResetRenderedModels()
{
	for (auto item : myModelRegistry)
	{
		if (item.second)
		{
			item.second->SetHasBeenRenderer(false);
		}
	}
}

bool ModelAssetHandler::LoadModelNewTesting(const std::wstring& aFilePath)
{
	HRESULT result = S_FALSE;
	ofbx::IScene* scene = LoadModelScene(Helpers::string_cast<std::string>(aFilePath));


	const ofbx::Mesh* mesh = nullptr;
	for (int i = 0; i < scene->getAllObjectCount(); ++i)
	{
		Ref<Model> mdl = MakeRef<Model>();
		const ofbx::Object* object = scene->getAllObjects()[i];
		if (object->getType() == ofbx::Object::Type::MESH)
		{
			mesh = static_cast<const ofbx::Mesh*>(object);

			if (!mesh)
			{
				return false;
			}

			// Access the vertex and index data from the mesh
			const ofbx::Geometry* geometry = mesh->getGeometry();
			const ofbx::Vec3* vertexPositions = geometry->getVertices();
			const int vertexCount = geometry->getVertexCount();
			const int* indices = geometry->getFaceIndices();
			const int indexCount = geometry->getIndexCount();

			std::vector<Vertex> mdlVertices(vertexCount);
			for (int x = 0; x < vertexCount; ++x)
			{
				mdlVertices[x].Position = Vector4f{
					static_cast<float>(vertexPositions[x].x),
					static_cast<float>(vertexPositions[x].y),
					static_cast<float>(vertexPositions[x].z),
					1.0f
				};



				for (int vCol = 0; vCol < 4; vCol++)
				{
					if (mesh->getGeometry()->getColors())
					{
						mdlVertices[x].VertexColors[vCol] = {
						static_cast<float>(mesh->getGeometry()->getColors()[0].x),
						static_cast<float>(mesh->getGeometry()->getColors()[0].y),
						static_cast<float>(mesh->getGeometry()->getColors()[0].z),
						static_cast<float>(mesh->getGeometry()->getColors()[0].w)
						};
					}
				}

				for (int uvCh = 0; uvCh < 4; uvCh++)
				{
					if (mesh->getGeometry()->getUVs(uvCh))
					{
						mdlVertices[x].UVs[uvCh].x = static_cast<float>(mesh->getGeometry()->getUVs(uvCh)->x);
						mdlVertices[x].UVs[uvCh].y = static_cast<float>(mesh->getGeometry()->getUVs(uvCh)->y);
					}

				}

				if (mesh->getGeometry()->getTangents())
				{
					mdlVertices[x].Tangent = Vector3f{

						static_cast<float>(mesh->getGeometry()->getTangents()[x].x),
						static_cast<float>(mesh->getGeometry()->getTangents()[x].y),
						static_cast<float>(mesh->getGeometry()->getTangents()[x].z)
					};
				}

				//mdlVertices[v].Binormal = {
				//	mesh.Vertices[v].BiNormal[0],
				//	mesh.Vertices[v].BiNormal[1],
				//	mesh.Vertices[v].BiNormal[2]
				//};

				if (mesh->getGeometry()->getNormals())
				{
					mdlVertices[x].Normal = {
						static_cast<float>(mesh->getGeometry()->getNormals()[x].x),
						static_cast<float>(mesh->getGeometry()->getNormals()[x].y),
						static_cast<float>(mesh->getGeometry()->getNormals()[x].z)
					};
				}

			}

			std::vector<unsigned int> mdlIndices(indexCount);
			for (int i = 0; i < indexCount; ++i)
			{
				mdlIndices[i] = indices[i];
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


			auto layout = CreateLayout();

			std::ifstream vsFile;
			vsFile.open("Shaders/Default_VS.cso", std::ios::binary);
			std::string vsData = { std::istreambuf_iterator<char>(vsFile), std::istreambuf_iterator<char>() };
			ID3D11VertexShader* vertexShader;
			result = DX11::Get().GetDevice()->CreateVertexShader(vsData.data(), vsData.size(), nullptr, &vertexShader);
			if (FAILED(result))
			{
				return false;
			}
			vsFile.close();

			std::ifstream psFile;
			psFile.open("Shaders/Default_PS.cso", std::ios::binary);
			std::string psData = { std::istreambuf_iterator<char>(psFile), std::istreambuf_iterator<char>() };
			ID3D11PixelShader* pixelShader;
			result = DX11::Get().GetDevice()->CreatePixelShader(psData.data(), psData.size(), nullptr, &pixelShader);
			if (FAILED(result))
			{
				return false;
			}
			psFile.close();

			ID3D11InputLayout* inputLayout;
			result = DX11::Get().GetDevice()->CreateInputLayout(layout.data(), sizeof(layout) / sizeof(D3D11_INPUT_ELEMENT_DESC), vsData.data(), vsData.size(), &inputLayout);
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
			modelData.myMeshName = mesh->name;
			modelData.myMaterialIndex = 0;


			mdl->Init(modelData, aFilePath);
		}

		mdl->Name = Helpers::string_cast<std::string>(aFilePath);
		Ref<ModelInstance> mdlInstance = MakeRef<ModelInstance>();

		mdlInstance->Init(mdl);

		myModelRegistry.insert({ aFilePath, mdlInstance });

		return true;
	}





	return false;
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


	const std::string ansiFileName = Helpers::string_cast<std::string>(aFilePath);

	HRESULT result = S_FALSE;

	std::vector<std::string> addedBlendShapes;


	TGA::FBX::Mesh tgaModel;
	importer.InitImporter();
	if (importer.LoadModel(aFilePath, tgaModel))
	{
		Ref<Model> mdl = MakeRef<Model>();

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


			for (size_t i = 0; i < 3; i++)
			{
				mdl->BoxBounds.BoxExtents.push_back(tgaModel.BoxSphereBounds.BoxExtents[i]);
				mdl->BoxBounds.Center.push_back(tgaModel.BoxSphereBounds.Center[i]);
			}
			mdl->BoxBounds.Radius = tgaModel.BoxSphereBounds.Radius;

			int size = static_cast<int>(mesh.Indices.size());
			mdlIndices.resize(size);
			memcpy_s(&mdlIndices[0], sizeof(unsigned) * size, &mesh.Indices[0], sizeof(unsigned) * size);


			/*for (int i = 0; i < mesh.Indices.size(); i++)
			{


				mdlIndices.push_back(mesh.Indices[i]);
			}*/


			Skeleton mdlSkeleton;
			const bool hasSkeleton = tgaModel.Skeleton.GetRoot();
			if (hasSkeleton)
			{
				for (size_t i = 0; i < tgaModel.Skeleton.Bones.size(); i++)
				{
					mdlSkeleton.Bones.push_back(Bone());
					mdlSkeleton.Bones[i].BindPoseInverse = *((Matrix4x4f*)&tgaModel.Skeleton.Bones[i].BindPoseInverse);
					mdlSkeleton.Bones[i].Children = tgaModel.Skeleton.Bones[i].Children;
					//mdlSkeleton.Bones[i].Name = tgaModel.Skeleton.Bones[i].Name;
					mdlSkeleton.Bones[i].ParentIdx = tgaModel.Skeleton.Bones[i].ParentIdx;
					mdlSkeleton.BoneNames.push_back(tgaModel.Skeleton.Bones[i].Name);
					mdlSkeleton.BoneNameToIndex[tgaModel.Skeleton.Bones[i].Name] = i;
				}
			}

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


			auto layout = CreateLayout();

			std::ifstream vsFile;
			vsFile.open("Shaders/Default_VS.cso", std::ios::binary);
			std::string vsData = { std::istreambuf_iterator<char>(vsFile), std::istreambuf_iterator<char>() };
			ID3D11VertexShader* vertexShader;
			result = DX11::Get().GetDevice()->CreateVertexShader(vsData.data(), vsData.size(), nullptr, &vertexShader);
			if (FAILED(result))
			{
				return false;
			}
			vsFile.close();

			std::ifstream psFile;
			psFile.open("Shaders/Default_PS.cso", std::ios::binary);
			std::string psData = { std::istreambuf_iterator<char>(psFile), std::istreambuf_iterator<char>() };
			ID3D11PixelShader* pixelShader;
			result = DX11::Get().GetDevice()->CreatePixelShader(psData.data(), psData.size(), nullptr, &pixelShader);
			if (FAILED(result))
			{
				return false;
			}
			psFile.close();

			ID3D11InputLayout* inputLayout;
			result = DX11::Get().GetDevice()->CreateInputLayout(layout.data(), sizeof(layout) / sizeof(D3D11_INPUT_ELEMENT_DESC), vsData.data(), vsData.size(), &inputLayout);
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

			Ref<Material> mat = MakeRef<Material>();
			mat->Init(Helpers::string_cast<std::wstring>(mesh.MeshName));
			mat->SetAlbedoTexture(albedo);
			mat->SetNormalTexture(normal);
			mat->SetMaterialTexture(material);

			mdl->PushMaterial(std::move(mat));



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

			if (hasSkeleton)
			{
				mdl->Init(modelData, aFilePath, mdlSkeleton);
			}
			else
			{
				mdl->Init(modelData, aFilePath);
			}


		}

		mdl->Name = ansiFileName;
		Ref<ModelInstance> mdlInstance = MakeRef<ModelInstance>();

		mdlInstance->Init(mdl);


		myModelRegistry.insert({ aFilePath, mdlInstance });

		return true;
	}

	return false;
#endif
}

bool ModelAssetHandler::LoadAnimationData(const std::wstring& aModelName, const std::wstring& someFilePath)
{
	const std::string ansiFileName = Helpers::string_cast<std::string>(someFilePath);
	Ref<ModelInstance> model = GetModelInstance(aModelName);

	TGA::FBX::Animation tgaAnimation;
	if (!TGA::FBX::Importer::LoadAnimation(someFilePath, tgaAnimation))
	{
		return false;
	}

	Animation animOut;
	animOut.Duration = static_cast<float>(tgaAnimation.Duration);
	animOut.FramesPerSecond = tgaAnimation.FramesPerSecond;
	animOut.Length = tgaAnimation.Length;
	animOut.Name = Helpers::string_cast<std::wstring>(tgaAnimation.Name);
	for (int i = 0; i < static_cast<int>(tgaAnimation.Length); i++)
	{
		Animation::Frame frames; //{ *(std::vector<Matrix4x4f>*)& tgaAnimation.Frames[i].LocalTransforms }
		for (int y = 0; y < tgaAnimation.Frames[i].LocalTransforms.size(); y++)
		{
			frames.LocalTransforms.push_back(*(Matrix4x4f*)&tgaAnimation.Frames[i].LocalTransforms);
		}

		animOut.Frames.push_back(frames);
	}

	model->GetSkeleton()->Animations.insert({ someFilePath, animOut });

	return true;
}

Ref<Model> ModelAssetHandler::GetModel(const std::wstring& aFilePath)
{
	auto model = myModelRegistry.find(aFilePath);

	if (model->second == nullptr)
	{
		LoadModelData(aFilePath);
		model = myModelRegistry.find(aFilePath);
	}

	return model->second->GetModel();
}

Ref<ModelInstance> ModelAssetHandler::GetModelInstance(const std::wstring& aFilePath)
{

	if (!std::filesystem::exists(aFilePath))
	{
		return nullptr;
	}


	{
		std::scoped_lock<std::mutex> lock(myListMutex);

		auto model = myModelRegistry.find(aFilePath);
		if (model != myModelRegistry.end())
		{
			return model->second;
		}
	}

	if (LoadModelData(aFilePath))
	{
		std::scoped_lock<std::mutex> lock(myListMutex);

		auto model = myModelRegistry.find(aFilePath);
		if (model != myModelRegistry.end())
		{
			return model->second;
		}

	}

	if (myModelRegistry.empty())
	{
		auto returnValue = MakeRef<ModelInstance>();
		returnValue->Init(MakeRef<Model>());
		return returnValue;
	}

	return nullptr;
}
