#include "GraphicsEngine.pch.h"
#include "Font.h"

#undef INFINITE
#include "msdf-atlas-gen.h"
#include "FontGeometry.h"
#include "Core/Model/Texture.h"
#include "Utilites/StringCast.h"
#include <Font/MSDFData.h>

namespace GamerEngine
{

	template<typename T, typename S, int N, msdf_atlas::GeneratorFunction<S,N> GenFunc>
	static Ref<Texture> CreateAndCacheAtlas(const std::string& aFontName,
		float aFontSize,
		const std::vector<msdf_atlas::GlyphGeometry>& aGlyphs,
		const msdf_atlas::FontGeometry& aFontGeometry, uint32_t aWidth, uint32_t aHeight)
	{
		msdf_atlas::GeneratorAttributes attributes;
		attributes.config.overlapSupport = true;
		attributes.scanlinePass = true;

		msdf_atlas::ImmediateAtlasGenerator<S, N, GenFunc, msdf_atlas::BitmapAtlasStorage<T, N>> generator(aWidth, aHeight);
		generator.setAttributes(attributes);
		generator.setThreadCount(8);
		generator.generate(aGlyphs.data(), (int)aGlyphs.size());

		msdfgen::BitmapConstRef<T, N> bitmap = (msdfgen::BitmapConstRef<T, N>)generator.atlasStorage();


		int y = 0;



		std::vector<CommonUtilities::Vector4<T>> newBuffer;

		uint32_t totalWidth = static_cast<uint32_t>(std::round(aWidth));
		uint32_t totalHeight = static_cast<uint32_t>(std::round(aHeight));
		uint32_t totalPixels = totalWidth * totalHeight;

		for (uint32_t i = 0; i < totalPixels; ++i) {
			uint32_t xIndex = i % totalWidth;
			uint32_t yIndex = totalHeight - 1 - (i / totalWidth); // Adjust for inverted y-coordinate

			CommonUtilities::Vector4<T> vectorStuff;

			auto data = bitmap(static_cast<uint32_t>(std::round(xIndex)), static_cast<uint32_t>(std::round(yIndex)));
			vectorStuff.x = data[0];
			vectorStuff.y = data[1];
			vectorStuff.z = data[2];
			vectorStuff.w = 255;

			newBuffer.push_back(vectorStuff);
		}


		TextureConfig config;
		config.Name = Helpers::string_cast<std::wstring>(aFontName);
		config.Width = bitmap.width;
		config.Height = bitmap.height;
		config.ColorCount = (N * sizeof(T)) + (1 * sizeof(T));
		config.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

		Ref<Texture> texture = TextureAssetHandler::CreateTexture(config, (void*)newBuffer.data());
		return texture;
	};

	GamerEngine::Font::Font(const std::filesystem::path& aFontPath) : myData(new GamerEngine::MSDFData())
	{
		msdfgen::FreetypeHandle* ft = msdfgen::initializeFreetype();
		std::string fileString = aFontPath.string();

		if (!ft)
		{
			return;
		}

		msdfgen::FontHandle* font = msdfgen::loadFont(ft, fileString.c_str());
		if (!font)
		{
			return;
		}


		struct CharsetRange
		{
			uint32_t Begin, End;
		};

		static const CharsetRange charsetRanges[] =
		{
			{0x0020, 0x00FF }
		};

		msdf_atlas::Charset charset;
		for (CharsetRange range : charsetRanges)
		{
			for (uint32_t c = range.Begin; c <= range.End; c++)
			{
				charset.add(c);
			}
		}

		double fontScale = 1.0;
		myData->FontGeometry = msdf_atlas::FontGeometry(&myData->Glyphs);
		int glyphsLoaded = myData->FontGeometry.loadCharset(font, fontScale, charset);


		double emSize = 40.0;

		msdf_atlas::TightAtlasPacker atlasPacker;
		// atlasPacker.setDimensioonsContraint();
		atlasPacker.setPixelRange(2.0);
		atlasPacker.setMiterLimit(1.0);
		atlasPacker.setPadding(0);
		atlasPacker.setScale(emSize);

		int remaning = atlasPacker.pack(myData->Glyphs.data(), (int)myData->Glyphs.size());
		if (remaning > 0)
		{
			// Pog
		}

		int width, height;
		atlasPacker.getDimensions(width, height);
		emSize = atlasPacker.getScale();


#define DEFAULT_ANGLE_THRESHOLD 3.0
#define LCG_MULTIPLIER 6364136223846793005ull
#define LCG_INCREMENT 1442695040888963407ull
#define THREAD_COUNT 8
		// if MSDF || MTSDF

		uint64_t coloringSeed = 0;
		bool expensiveColoring = false;
		if (expensiveColoring)
		{
			msdf_atlas::Workload([&glyphs = myData->Glyphs, &coloringSeed](int i, int threadNo) -> bool {
				unsigned long long glyphSeed = (LCG_MULTIPLIER * (coloringSeed ^ i) + LCG_INCREMENT) * !!coloringSeed;
				glyphs[i].edgeColoring(msdfgen::edgeColoringInkTrap, DEFAULT_ANGLE_THRESHOLD, glyphSeed);
				return true;
				}, static_cast<int>(myData->Glyphs.size())).finish(THREAD_COUNT);
		}
		else {
			unsigned long long glyphSeed = coloringSeed;
			for (msdf_atlas::GlyphGeometry& glyph : myData->Glyphs)
			{
				glyphSeed *= LCG_MULTIPLIER;
				glyph.edgeColoring(msdfgen::edgeColoringInkTrap, DEFAULT_ANGLE_THRESHOLD, glyphSeed);
			}
		}




		Ref<Texture> texture = CreateAndCacheAtlas<uint8_t, float, 3, msdf_atlas::msdfGenerator>("Ariel", (float)emSize, myData->Glyphs, myData->FontGeometry, width, height);

#if 0
		msdfgen::Shape shape;
		if (msdfgen::loadGlyph(shape, font, 'A'))
		{
			shape.normalize();
			msdfgen::edgeColoringSimple(shape, 3.0f);

			msdfgen::Bitmap<float, 3> msdf(32, 32);
			msdfgen::generateMSDF(msdf, shape, 4.0f, 1.0, msdfgen::Vector2(4.0f, 4.0f));
			msdfgen::savePng(msdf, "output.png");
		}
#endif


		

		msdfgen::destroyFont(font);
		msdfgen::deinitializeFreetype(ft);
	}

	Font::~Font()
	{
		delete myData;
	}
}
