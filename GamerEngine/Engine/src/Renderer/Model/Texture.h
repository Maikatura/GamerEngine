#pragma once
#include <filesystem>
#include <string>
#include <Renderer/AssetHandlers/TextureAssetHandler.h>
#include <wrl/client.h>

using namespace Microsoft::WRL;

class Texture
{
	friend class TextureAssetHandler;
	friend class PostProcessRenderer;

protected:
	std::wstring myName;
	std::wstring myPath;

	ComPtr<struct ID3D11Resource> myTexture;
	ComPtr<struct ID3D11ShaderResourceView> mySRV;

public:

	
	FORCEINLINE const ComPtr<struct ID3D11ShaderResourceView> GetSRV() const { return mySRV; }
	FORCEINLINE const std::wstring& GetName() const { return myName; }
	FORCEINLINE const std::wstring& GetPath() const { return myPath; }

	Texture();
	virtual ~Texture();

	void SetName(const std::wstring& aName);
	void SetAsResource(unsigned int aSlot);
	void SetPath(const std::wstring& aPath);
};
