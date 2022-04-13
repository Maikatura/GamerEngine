#pragma once

#include "import_base.h"

namespace GamerEngine {

	class ObjMeshImporter : public IMeshImporter
	{
	public:
		virtual bool from_file(const std::string& filepath, GamerEngine::Mesh* pMesh) override;

	};
}
