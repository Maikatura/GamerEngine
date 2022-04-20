#pragma once
#include "elems/mesh.h"



struct MeshComponent
{
	GamerEngine::Mesh myModel;

	MeshComponent() = default;
	MeshComponent(const MeshComponent&) = default;
};
