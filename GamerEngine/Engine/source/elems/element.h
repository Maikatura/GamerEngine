#pragma once
#include "shader/shader_util.h"

namespace GamerEngine
{
	  class Element
	  {
	  public:
	    virtual void Update(GamerEngine::Shader* shader) = 0;
	  };
}

