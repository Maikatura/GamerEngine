#pragma once
#include <string>
#include <AssetHandlers/TextureAssetHandler.h>
#include <wrl/client.h>

using namespace Microsoft::WRL;

class Texture
{
	friend class TextureAssetHandler;

protected:
	std::wstring myName;

	ComPtr<struct ID3D11Resource> myTexture;
	ComPtr<struct ID3D11ShaderResourceView> mySRV;

public:

	FORCEINLINE const ComPtr<struct ID3D11ShaderResourceView> GetSRV() const { return mySRV; }
	FORCEINLINE const std::wstring& GetName() const { return myName; }

	virtual ~Texture();

	void SetName(const std::wstring& aName);
	void SetAsResource(unsigned int aSlot);
};
