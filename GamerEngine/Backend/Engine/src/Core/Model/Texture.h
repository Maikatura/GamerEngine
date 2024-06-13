#pragma once
#include <filesystem>
#include <string>
#include <AssetHandlers/TextureAssetHandler.h>
#include <wrl/client.h>

#include "Core/Asset/Asset.h"


using namespace Microsoft::WRL;

enum class TextureType
{
	Albedo,
	Normal,
	Material,
	COUNT
};

class Texture : public GamerEngine::Asset
{
	friend class TextureAssetHandler;
	friend class PostProcessRenderer;

protected:

	int myWidth;
	int myHeight;

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


	int GetWidth() { return myWidth; }
	int GetHeight() { return myHeight; }

	Texture();
	virtual ~Texture();

	static GamerEngine::AssetType GetStaticType() { return GamerEngine::AssetType::Texture2D; }
	virtual GamerEngine::AssetType GetType() const override { return GetStaticType(); }

	void SetWidth(int aWidth) { myWidth = aWidth; }
	void SetHeight(int aHeight) { myHeight = aHeight; }
	void SetName(const std::wstring& aName);
	void SetAsResource(unsigned int aSlot);
	void ClearResource(unsigned int aSlot);
	void SetPath(const std::wstring& aPath);
};
