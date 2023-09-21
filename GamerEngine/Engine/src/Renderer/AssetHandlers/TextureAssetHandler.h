#pragma once
#include <unordered_map>
#include <Renderer/Model/Texture.h>
#include <memory>
#include <d3d11.h>

class Texture;
class DepthStencil;

class TextureAssetHandler
{
	typedef std::unordered_map<std::wstring, std::shared_ptr<Texture>> TextureMap;

	inline static TextureMap myRegistry{};

public:

	static void Clear();

	static std::shared_ptr<Texture> GetTexture(const std::wstring& aName);

	static bool LoadTexture(const std::wstring& aFileName);
	static void UnloadUnusedTextures();

	static std::shared_ptr<DepthStencil> CreateDepthStencil(const std::wstring& aName, size_t aWidth, size_t aHeight);

	static ID3D11PixelShader* GetPixelShader(const std::string& aPath);
	static ID3D11VertexShader* GetVertexShader(const std::string& aPath);
	static std::vector<std::shared_ptr<Texture>> GetTextureList();
};
