#pragma once
#include <Math/MathTypes.hpp>

struct Vertex
{
	Vector4f Position = { 0,0,0,1 };
	Vector4f VertexColors[4] =
	{
		{1,1,1,1},
		{1,1,1,1},
		{1,1,1,1},
		{1,1,1,1}
	};

	Vector2f UVs[4]
	{
		{0,0},
		{0,0},
		{0,0},
		{0,0}
	};

	Vector4ui myBoneIDs = {0,0,0,0};
	Vector4f myBoneWeights = { 0,0,0,0 };
};
