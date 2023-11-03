#pragma once
#include <string>
#include "Texture.h"
#include <Math/MathTypes.hpp>
#include <d3d11.h>




class Material
{
public:
	struct MaterialData
	{
		Vector3f Albedo = { 1.0f,1.0f,1.0f };
		float padding_1;
	};

private:
	std::array<Ref<Texture>, static_cast<int>(TextureType::COUNT)> myTextures;
	MaterialData myMaterialData;
	std::wstring myName;

public:
	Material();
	virtual ~Material() = default;

	Material(const Material& other) = default;

	void Init(std::wstring aName);
	void Init(std::wstring aName, const Vector3f& anAlbedo);

	FORCEINLINE const std::wstring GetName() const { return myName; }
	FORCEINLINE const Vector3f& GetAlbedo() const { return myMaterialData.Albedo; }

	FORCEINLINE const Ref<Texture>& GetAlbedoTexture() const { return myTextures[static_cast<int>(TextureType::Albedo)];  }
	FORCEINLINE const Ref<Texture>& GetNormalTexture() const { return myTextures[static_cast<int>(TextureType::Normal)]; }
	FORCEINLINE const Ref<Texture>& GetMaterialTexture() const { return myTextures[static_cast<int>(TextureType::Material)]; }

	void SetTexture(TextureType aTextureType, Ref<Texture> aTexture);

	void SetAlbedoTexture(Ref<Texture> aTexture);
	void SetNormalTexture(Ref<Texture> aNormalTexture);
	void SetMaterialTexture(Ref<Texture> aMaterialTexture);

	void SetAsResource(ComPtr<ID3D11Resource> aMaterialBuffer);

};
