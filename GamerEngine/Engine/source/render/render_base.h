#pragma once

#include "elems/vertex_holder.h"

#include "window/window.h"

namespace GamerEngine
{
	class VertexIndexBuffer
	{
	public:
		VertexIndexBuffer() : mVBO{ 0 }, mVAO{ 0 }, mIBO{ 0 }
		{}

		virtual void CreateBuffers(const std::vector<GamerEngine::VertexHolder>& vertices, const std::vector<unsigned int>& indices) = 0;

		virtual void DeleteBuffers() = 0;

		virtual void Bind() = 0;

		virtual void Unbind() = 0;

		virtual void Draw(int index_count) = 0;

	protected:
		GLuint mVBO;
		GLuint mVAO;
		GLuint mIBO;
	};

	class FrameBuffer
	{
	public:
		FrameBuffer() : mFBO{ 0 }, mDepthId{ 0 }
		{}

		virtual void CreateBuffers(int32_t width, int32_t height) = 0;

		virtual void DeleteBuffers() = 0;

		virtual void Bind() = 0;

		virtual void Unbind() = 0;

		virtual uint32_t GetTexture() = 0;

	protected:
		uint32_t mFBO = 0;
		uint32_t mTexId = 0;
		uint32_t mDepthId = 0;
		int32_t mWidth = 0;
		int32_t mHeight = 0;
	};

	class RenderContext
	{

	public:

		RenderContext() : mWindow(nullptr) {}

		virtual bool Init(GamerEngine::IWindow* window)
		{
			mWindow = window;
			return true;
		}

		virtual void PreRender() = 0;

		virtual void PostRender() = 0;

		virtual void end() = 0;

	protected:
		GamerEngine::IWindow* mWindow;
	};
}
