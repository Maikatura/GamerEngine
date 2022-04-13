#include "pch.h"
#include "mesh.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include "render/opengl_buffer_manager.h"

namespace GamerEngine
{
	void Mesh::Init()
	{
		mRenderBufferMgr = std::make_unique<GamerEngine::OpenGL_VertexIndexBuffer>();

		CreateBuffers();
	}

	Mesh::~Mesh()
	{
		DeleteBuffers();
	}

	bool Mesh::Load(const std::string& filepath)
	{
		const uint32_t cMeshImportFlags =
			aiProcess_CalcTangentSpace |
			aiProcess_Triangulate |
			aiProcess_SortByPType |
			aiProcess_GenNormals |
			aiProcess_GenUVCoords |
			aiProcess_OptimizeMeshes |
			aiProcess_ValidateDataStructure;

		Assimp::Importer Importer;

		const aiScene* pScene = Importer.ReadFile(filepath.c_str(),
			cMeshImportFlags);

		if (pScene && pScene->HasMeshes())
		{
			mVertexIndices.clear();
			mVertices.clear();

			auto* mesh = pScene->mMeshes[0];

			for (uint32_t i = 0; i < mesh->mNumVertices; i++)
			{
				VertexHolder vh;
				vh.mPos = { mesh->mVertices[i].x, mesh->mVertices[i].y ,mesh->mVertices[i].z };
				vh.mNormal = { mesh->mNormals[i].x, mesh->mNormals[i].y ,mesh->mNormals[i].z };


				AddVertex(vh);
			}

			for (size_t i = 0; i < mesh->mNumFaces; i++)
			{
				aiFace face = mesh->mFaces[i];

				for (size_t j = 0; j < face.mNumIndices; j++)
					AddVertexIndex(face.mIndices[j]);
			}

			Init();
			return true;
		}
		return false;
	}

	void Mesh::Update(nshaders::Shader* shader)
	{
		shader->set_vec3(mColor, "albedo");
		shader->set_f1(mRoughness, "roughness");
		shader->set_f1(mMetallic, "metallic");
		shader->set_f1(1.0f, "ao");
	}

	void Mesh::CreateBuffers()
	{
		mRenderBufferMgr->CreateBuffers(mVertices, mVertexIndices);
	}

	void Mesh::DeleteBuffers()
	{
		mRenderBufferMgr->DeleteBuffers();
	}

	void Mesh::Bind()
	{
		mRenderBufferMgr->Bind();
	}

	void Mesh::Unbind()
	{
		mRenderBufferMgr->Unbind();
	}

	void Mesh::Render()
	{
		mRenderBufferMgr->Draw((int)mVertexIndices.size());
	}
}