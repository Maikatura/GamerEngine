#pragma once

#include "render_base.h"

namespace nrender
{
  class UIContext : public RenderContext
  {

  public:

    bool Init(nwindow::IWindow* window) override;

    void PreRender() override;

    void PostRender() override;

    void end() override;

  };
}