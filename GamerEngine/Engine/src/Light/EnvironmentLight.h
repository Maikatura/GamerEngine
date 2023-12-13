#pragma once
#include <Light/Light.h>
#include "Model/Texture.h"


class EnvironmentLight : public Light
{
	friend class LightAssetHandler;

	Ref<Texture> myTexture;


public:


	virtual void SetAsResource(Microsoft::WRL::ComPtr<ID3D11Buffer> aLightBuffer, int aShaderIndex) override;

	void SetData(TransformComponent* data) override;
};
