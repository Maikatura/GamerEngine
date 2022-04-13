#pragma once
#include "shader/shader_util.h"

namespace GamerEngine
{
	  class Element
	  {
	  public:
	    virtual void update(nshaders::Shader* shader) = 0;
	  };
}

