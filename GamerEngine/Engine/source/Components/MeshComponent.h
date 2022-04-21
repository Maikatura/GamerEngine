#pragma once
#include "elems/mesh.h"



struct MeshComponent
{
	GamerEngine::Mesh myModel;
	glm::vec4 myColor;

	MeshComponent() = default;
	MeshComponent(const MeshComponent&) = default;
};
