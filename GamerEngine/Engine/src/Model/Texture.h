#pragma once
#include <filesystem>
#include <string>
#include <AssetHandlers/TextureAssetHandler.h>
#include <wrl/client.h>


using namespace Microsoft::WRL;

enum class TextureType
{
	Albedo,
	Normal,
	Material,
	COUNT
};

class Texture
{
	friend class TextureAssetHandler;
	friend class PostProcessRenderer;

protected:
	std::wstring myName;
	std::wstring myPath;

	TextureType myTextureType;

	ComPtr<struct ID3D11Resource> myTexture;
	ComPtr<struct ID3D11ShaderResourceView> mySRV;

public:

	
	
	FORCEINLINE const ComPtr<struct ID3D11ShaderResourceView> GetSRV() const { return mySRV; }
	FORCEINLINE const std::wstring& GetName() const { return myName; }
	FORCEINLINE const std::wstring& GetPath() const { return myPath; }
	TextureType GetTextureType();
	void SetTextureType(TextureType aTextureType);

	Texture();
	virtual ~Texture();

	void SetName(const std::wstring& aName);
	void SetAsResource(unsigned int aSlot);
	void ClearResource(unsigned int aSlot);
	void SetPath(const std::wstring& aPath);
};
