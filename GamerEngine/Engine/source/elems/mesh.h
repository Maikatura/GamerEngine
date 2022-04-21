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

		Mesh();
		Mesh(const Mesh&) = default;

		virtual ~Mesh();

		bool Load(const std::string& filepath);
		void AddVertex(const VertexHolder& vertex) { mVertices.push_back(vertex); }
		void AddVertexIndex(unsigned int vertex_idx) { mVertexIndices.push_back(vertex_idx); }
		std::vector<unsigned int> GetVertexIndices() { return mVertexIndices; }
		void Update(TransformComponent aTransform, glm::vec4 aColor , GamerEngine::Shader* shader);
		void Update(GamerEngine::Shader* shader) override;

		glm::vec3 mColor = { 1.0f, 0.0f, 0.0f };
		float mRoughness = 0.2f;
		float mMetallic = 0.1f;

		void Init();
		void CreateBuffers();
		void DeleteBuffers();
		void Render();
		void Bind();
		void Unbind();
	private:
		// Buffers manager
		std::shared_ptr<GamerEngine::VertexIndexBuffer> mRenderBufferMgr;

		// Vertices and indices
		std::vector<VertexHolder> mVertices;
		std::vector<unsigned int> mVertexIndices;
	};
}

