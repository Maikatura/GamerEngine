#pragma once
#include <string>
#include "Texture.h"
#include <Math/MathTypes.hpp>
#include <d3d11.h>

namespace MaterialTextureChannel
{
	enum
	{
		Albedo,
		Normal,
		Material,
		COUNT
	};
};

class Material
{
public:
	struct MaterialData
	{
		Vector3f Albedo = { 1.0f,1.0f,1.0f };
		float padding_1;
	};

private:
	std::array<std::shared_ptr<Texture>, MaterialTextureChannel::COUNT> myTextures;
	MaterialData myMaterialData;
	std::wstring myName;

public:
	Material();
	virtual ~Material() = default;

	Material(const Material& other) = default;

	void Init(std::wstring aName, const Vector3f& anAlbedo);

	FORCEINLINE const std::wstring& GetName() const { return myName; }
	FORCEINLINE const Vector3f& GetAlbedo() const { return myMaterialData.Albedo; }

	FORCEINLINE const std::shared_ptr<Texture>& GetAlbedoTexture() const { return myTextures[MaterialTextureChannel::Albedo];  }
	FORCEINLINE const std::shared_ptr<Texture>& GetNormalTexture() const { return myTextures[MaterialTextureChannel::Normal]; }
	FORCEINLINE const std::shared_ptr<Texture>& GetMaterialTexture() const { return myTextures[MaterialTextureChannel::Material]; }

	void SetAlbedoTexture(std::shared_ptr<Texture> aTexture);
	void SetNormalTexture(std::shared_ptr<Texture> aNormalTexture);
	void SetMaterialTexture(std::shared_ptr<Texture> aMaterialTexture);

	void SetAsResource(ComPtr<ID3D11Resource> aMaterialBuffer);

};
