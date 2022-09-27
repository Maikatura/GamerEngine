#pragma once
#include "Math/MathTypes.hpp"
#include "Model/ModelInstance.h"


class RenderBuffer
{
public:
	Matrix4x4f myTransform;
	std::shared_ptr<ModelInstance> myModel;


};
