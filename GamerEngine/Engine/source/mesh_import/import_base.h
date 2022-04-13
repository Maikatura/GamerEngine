#pragma once
#include "pch.h"
#include "../elems/mesh.h"

namespace GamerEngine {

	struct IMeshImporter
	{
		virtual bool from_file(const std::string& filepath, GamerEngine::Mesh* pMesh) = 0;
	};
}