#pragma once
#include <unordered_map>
#include <Model/Texture.h>
#include <memory>

class Texture;

class TextureAssetHandler
{
	typedef std::unordered_map<std::wstring, std::shared_ptr<Texture>> TextureMap;

	inline static TextureMap myRegistry{};

public:
	static std::shared_ptr<Texture> GetTexture(const std::wstring& aName);

	static bool LoadTexture(const std::wstring& aFileName);
	static void UnloadUnusedTextures();
};
