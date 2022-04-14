#pragma once
#include "pch.h"
#include "render/render_base.h"
#include "vertex_holder.h"
#include "elems/element.h"
#include "Components/TransformComponent.h"


namespace GamerEngine
{

	class Mesh : public Element
	{

	public:

		Mesh() = default;
		virtual ~Mesh();

		bool Load(const std::string& filepath);
		void AddVertex(const VertexHolder& vertex) { mVertices.push_back(vertex); }
		void AddVertexIndex(unsigned int vertex_idx) { mVertexIndices.push_back(vertex_idx); }
		std::vector<unsigned int> GetVertexIndices() { return mVertexIndices; }
		void Update(nshaders::Shader* shader) override;

		glm::vec3 mColor = { 1.0f, 0.0f, 0.0f };
		float mRoughness = 0.2f;
		float mMetallic = 0.1f;

		void Init();
		void CreateBuffers();
		void DeleteBuffers();
		void Render();
		void Bind();
		void Unbind();

		TransformComponent myTransform = {}; // TODO:  CHANGE THIS TO TRANSFORM COMPONENT ON ENTITY!
	private:
		// Buffers manager
		std::unique_ptr<GamerEngine::VertexIndexBuffer> mRenderBufferMgr;

		// Vertices and indices
		std::vector<VertexHolder> mVertices;
		std::vector<unsigned int> mVertexIndices;
	};
}

