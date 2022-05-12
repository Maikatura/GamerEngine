#include "pch.h"
#include "mesh.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include "ECS/Scene.h"
#include "render/opengl_buffer_manager.h"

namespace GamerEngine
{
	void Mesh::Init()
	{
		if (!mRenderBufferMgr)
		{
			mRenderBufferMgr = std::make_shared<GamerEngine::OpenGL_VertexIndexBuffer>();
		}
		else
		{
			mRenderBufferMgr->DeleteBuffers();
		}


		CreateBuffers();
	}

	Mesh::Mesh()
	{
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

	void Mesh::Update(TransformComponent aTransform, glm::vec4 aColor, GamerEngine::Shader* shader)
	{
		glm::mat4 position(1.0f);
		glm::mat4 rotation(1.0f);
		glm::mat4 scale(1.0f);
		position = glm::translate(position, { aTransform.myPosition.x, aTransform.myPosition.y, aTransform.myPosition.z });
		rotation = glm::rotate(rotation, glm::radians(aTransform.myRotation.x), glm::vec3(1, 0, 0));
		rotation = glm::rotate(rotation, glm::radians(aTransform.myRotation.y), glm::vec3(0, 1, 0));
		rotation = glm::rotate(rotation, glm::radians(aTransform.myRotation.z), glm::vec3(0, 0, 1));
		scale = glm::scale(scale, { aTransform.myScale.x, aTransform.myScale.y, aTransform.myScale.z });

		glm::mat4 modelView = position * scale * rotation;

		shader->set_mat4(modelView, "model");
		shader->set_mat4(Scene::GetCamera()->GetViewMatrix(), "view");
		shader->set_mat4(Scene::GetCamera()->GetProjection(), "projection");
		shader->set_vec4(aColor, "color");
	}

	void Mesh::Update(GamerEngine::Shader* shader)
	{
		// This does nothing :)
	}

	void Mesh::CreateBuffers()
	{
		mRenderBufferMgr->CreateBuffers(mVertices, mVertexIndices);
	}

	void Mesh::DeleteBuffers()
	{
		if (mRenderBufferMgr)
		{
			mRenderBufferMgr->DeleteBuffers();
		}
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
		if (mRenderBufferMgr)
		{
			mRenderBufferMgr->Draw((int)mVertexIndices.size());
		}
	}
}