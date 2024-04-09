#include "GraphicsEngine.pch.h"
//#include "FontAssetHandler.h"
//
//
//#include "FontAssetHandler.h"
//#include "TextureAssetHandler.h"
//#include <fstream>
//
//#include "nlohmann/json.hpp"
//
//UnqRef<LLL::FontAssetHandler> LLL::FontAssetHandler::ourInstance;
//void LLL::FontAssetHandler::StartUp()
//{
//	ourInstance = MakeUnqRef<FontAssetHandler>();
//}
//
//void LLL::FontAssetHandler::Shutdown()
//{
//	ourInstance.reset();
//}
//
//Ref<GamerEngine::Font> LLL::FontAssetHandler::GetFont(const std::filesystem::path& aPath)
//{
//	std::string id = aPath.string();
//	auto fontIt = myFontRegistry.find(id);
//	if (fontIt != myFontRegistry.end())
//	{
//		return fontIt->second;
//	}
//	return nullptr;
//}
//
//Ref<GamerEngine::Font> LLL::FontAssetHandler::GetOrLoadFont(const std::filesystem::path& aPath)
//{
//	std::string id = aPath.string();
//	auto fontIt = myFontRegistry.find(id);
//
//	if (fontIt == myFontRegistry.end())
//	{
//		if (LoadFontFromFile(aPath))
//		{
//			return myFontRegistry.at(id);
//		}
//
//		//LOG_ERROR("Failed to load font at {0}", aPath.string());
//		return nullptr;
//	}
//
//	return fontIt->second;
//}
//
//bool LLL::FontAssetHandler::LoadFontFromFile(const std::filesystem::path& aPath)
//{
//	//StringID id = aPath;
//	const std::string fontDefinitionName = aPath.parent_path().string() + "/" + aPath.stem().string() + ".json";
//	std::ifstream fontDefinitionStream(fontDefinitionName);
//	if (!fontDefinitionStream.good())
//	{
//		//LOG_ERROR("Failed to find font definition json file: ", fontDefinitionName);
//		return false;
//	}
//	nlohmann::json fontDefinition;
//	fontDefinitionStream >> fontDefinition;
//	fontDefinitionStream.close();
//
//	Ref<GamerEngine::Font> font = MakeRef<Font>();
//	font->Atlas.Size = fontDefinition["atlas"]["size"];
//	font->Atlas.Width = fontDefinition["atlas"]["width"];
//	font->Atlas.Height = fontDefinition["atlas"]["height"];
//	font->Atlas.EmSize = fontDefinition["metrics"]["emSize"];
//	font->Atlas.LineHeight = fontDefinition["metrics"]["lineHeight"];
//	font->Atlas.Ascender = fontDefinition["metrics"]["ascender"];
//	font->Atlas.Descender = fontDefinition["metrics"]["descender"];
//
//	for (auto& glyph : fontDefinition["glyphs"])
//	{
//		char unicode = static_cast<char>(glyph.value("unicode", 0));
//		float advance = glyph.value("advance", -1.0f);
//
//		Vector4f planeBounds = Vector4f(0, 0, 0, 0);
//		if (glyph.find("planeBounds") != glyph.end())
//		{
//			planeBounds.x = glyph["planeBounds"]["left"];
//			planeBounds.y = glyph["planeBounds"]["bottom"];
//			planeBounds.z = glyph["planeBounds"]["right"];
//			planeBounds.w = glyph["planeBounds"]["top"];
//		}
//		Vector4f uvBounds = Vector4f(0, 0, 0, 0);
//		if (glyph.find("atlasBounds") != glyph.end())
//		{
//			const float UVStartX = glyph["atlasBounds"]["left"] / static_cast<float>(font->Atlas.Width);
//			const float UVStartY = glyph["atlasBounds"]["top"] / static_cast<float>(font->Atlas.Height);
//			const float UVEndX = glyph["atlasBounds"]["right"] / static_cast<float>(font->Atlas.Width);
//			const float UVEndY = glyph["atlasBounds"]["bottom"] / static_cast<float>(font->Atlas.Height);
//
//			uvBounds = { UVStartX, UVStartY, UVEndX, UVEndY };
//		}
//		font->Glyphs.insert({ unicode, {unicode, advance, planeBounds, uvBounds} });
//	}
//
//	auto fontTexture = TextureAssetHandler::GetTexture(aPath);
//	if (!fontTexture)
//	{
//		//LOG_ERROR("Font texture failed to load: ", aPath.string());
//		return false;
//	}
//	font->Resource = fontTexture;
//	myFontRegistry[aPath.string()] = font;
//	return static_cast<bool>(font);
//}
