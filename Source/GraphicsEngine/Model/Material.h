#pragma once
#include <string>

#include "Texture.h"
#include "Math/MathTypes.hpp"
#include <d3d11.h>

enum MaterialTextureChannel
{
	Albedo,
	COUNT
};

class Material
{
	std::wstring myName;

public:
	struct MaterialData
	{
		Vector3f Albedo;
		float padding;
	};

private:
	MaterialData myMaterialData;

	std::array<std::shared_ptr<Texture>, MaterialTextureChannel::COUNT> myTextures;

public:
	Material();
	virtual ~Material() = default;

	void Init(std::wstring aName, const Vector3f& anAlbedo);

	FORCEINLINE const std::wstring& GetName() const { return myName; }
	FORCEINLINE const Vector3f& GetAlbedo() const { return myMaterialData.Albedo; }

	void SetAlbedoTexture(std::shared_ptr<Texture> aTexture);
	void SetAsResource(ComPtr<ID3D11Resource> aMaterialBuffer);

};
