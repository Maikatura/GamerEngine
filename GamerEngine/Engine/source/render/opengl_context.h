#pragma once

#include "render_base.h"

namespace nrender
{
  class OpenGL_Context : public RenderContext
  {
  public:

    bool init(nwindow::IWindow* window) override;

    void PreRender() override;

    void PostRender() override;

    void end() override;
  };
}
