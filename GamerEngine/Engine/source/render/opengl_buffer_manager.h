#pragma once

#include "pch.h"

#include "render_base.h"

namespace GamerEngine
{
  class OpenGL_VertexIndexBuffer : public VertexIndexBuffer
  {
  public:
    OpenGL_VertexIndexBuffer() : VertexIndexBuffer()
    {}

    void CreateBuffers(const std::vector<GamerEngine::VertexHolder>& vertices, const std::vector<unsigned int>& indices) override;

    void DeleteBuffers() override;

    void Bind() override;

    void Unbind() override;

    void Draw(int index_count) override;

  };

  class OpenGL_FrameBuffer : public FrameBuffer
  {
  public:

    void CreateBuffers(int32_t width, int32_t height) override;

    void DeleteBuffers() override;

    void Bind() override;

    void Unbind() override;
    
    uint32_t GetTexture() override;
  };
}
