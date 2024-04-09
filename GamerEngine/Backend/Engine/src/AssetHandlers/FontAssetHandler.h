//#pragma once
//#include <filesystem>
//#include <memory>
//#include <unordered_map>
//#include "Data/UniquePtr.h"
//#include "Font/Font.h"
//
//namespace LLL
//{
//	struct Vertex;
//
//	class FontAssetHandler
//	{
//		friend class AssetManager;
//	public:
//		static void StartUp();
//		static void Shutdown();
//
//		Ref<GamerEngine::Font> GetFont(const std::filesystem::path& aPath);
//
//		Ref<GamerEngine::Font> GetOrLoadFont(const std::filesystem::path& aPath);
//
//		bool LoadFontFromFile(const std::filesystem::path& aPath);
//
//		static FontAssetHandler& Get() { return *ourInstance; }
//	private:
//		std::unordered_map<std::string, Ref<GamerEngine::Font>> myFontRegistry;
//		static UnqRef<FontAssetHandler> ourInstance;
//	};
//}