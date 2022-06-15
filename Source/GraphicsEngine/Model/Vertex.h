#pragma once
#include "Math/Vector4.hpp"

struct Vertex
{
	CommonUtilities::Vector4<float> Position = { 0,0,0,1 };

	CommonUtilities::Vector4<float> VertexColors[4] =
	{
		{0,0,0,0},
		{0,0,0,0},
		{0,0,0,0},
		{0,0,0,0}
	};
};