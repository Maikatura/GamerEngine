#include "GraphicsEngine.pch.h"
#include "Font.h"

#undef INFINITE
#include "msdf-atlas-gen.h"


namespace Engine
{

	

	Font::Font(const std::filesystem::path& aFontPath)
	{
		msdfgen::FreetypeHandle* ft = msdfgen::initializeFreetype();



		if (!ft)
		{
			
			return;
		}

		std::string fileString = aFontPath.string();
		msdfgen::FontHandle* font = msdfgen::loadFont(ft, fileString.c_str());
		if (!font)
		{
			
			return;
		}

		msdfgen::Shape shape;
		if (msdfgen::loadGlyph(shape, font, 'A'))
		{
			shape.normalize();
			msdfgen::edgeColoringSimple(shape, 3.0f);

			msdfgen::Bitmap<float, 3> msdf(32, 32);
			msdfgen::generateMSDF(msdf, shape, 4.0f, 1.0, msdfgen::Vector2(4.0f, 4.0f));
			msdfgen::savePng(msdf, "output.png");
		}

		msdfgen::destroyFont(font);
		msdfgen::deinitializeFreetype(ft);
	}
}
