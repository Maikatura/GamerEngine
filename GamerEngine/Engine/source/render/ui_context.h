#pragma once

#include "render_base.h"

namespace GamerEngine
{
  class UIContext : public RenderContext
  {

  public:

    bool Init(GamerEngine::IWindow* window) override;

    void PreRender() override;

    void PostRender() override;

    void end() override;

  };
}