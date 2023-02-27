#pragma once
#include <Renderer/Light/Light.h>
#include "Renderer/Model/Texture.h"


class EnvironmentLight : public Light
{
	friend class LightAssetHandler;

	std::shared_ptr<Texture> myTexture;


public:


	virtual void SetAsResource(Microsoft::WRL::ComPtr<ID3D11Buffer> aLightBuffer) override;

	void SetData(TransformComponent* data) override;
};
