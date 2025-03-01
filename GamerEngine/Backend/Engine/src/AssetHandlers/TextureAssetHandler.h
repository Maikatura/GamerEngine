#pragma once
#include <unordered_map>
#include <memory>
#include <d3d11.h>

#include "Utilites/Pointers.h"

class Texture;
class DepthStencil;


struct TextureConfig
{
	std::wstring Name;
	int Width;
	int Height;
	int ColorCount;
	int Format;
};

class TextureAssetHandler
{
	typedef std::unordered_map<std::wstring, Ref<Texture>> TextureMap;

	inline static TextureMap myRegistry{};

public:

	static void Clear();


	static Ref<Texture> CreateTexture(const TextureConfig& aName, void* aPixels);
	static Ref<Texture> CreateTexture(const std::wstring& aName, DXGI_FORMAT aFormat, void* aPixels, int aWidth, int aHeight, int aColorCount = 4);
	static Ref<Texture> GetTexture(const std::wstring& aName);

	static bool LoadTexture(const std::wstring& aFileName);
	static void UnloadUnusedTextures();

	static Ref<DepthStencil> CreateDepthStencil(const std::wstring& aName, size_t aWidth, size_t aHeight);
	static Ref<DepthStencil> CreatePointLightMap(const std::wstring& aName, size_t aWidth, size_t aHeight);

	static ID3D11PixelShader* GetPixelShader(const std::string& aPath);
	static ID3D11VertexShader* GetVertexShader(const std::string& aPath);
	static std::vector<Ref<Texture>> GetTextureList();
};

