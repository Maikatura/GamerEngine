#pragma once
#undef INFINITE
#include "FontGeometry.h"
#include "GlyphGeometry.h"

namespace GamerEngine
{
	

struct MSDFData
{
	std::vector<msdf_atlas::GlyphGeometry> Glyphs;
	msdf_atlas::FontGeometry FontGeometry;
};

}
