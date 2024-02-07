#pragma once
#include "Light.h"
#include "Components/Camera.h"


class PointLight : public Light
{
	friend class LightAssetHandler;

public:
	virtual ~PointLight() override = default;


	void SetAsResource(Microsoft::WRL::ComPtr<ID3D11Buffer> aLightBuffer, unsigned int aShaderIndex) override;

	void Update() override;

	void SetData(TransformComponent* aTransform) override;

	Camera myCamera;
};
