#pragma once
#include <Light/Light.h>

class TransformComponent;
class DirectionalLightComponent;

class DirectionalLight : public Light
{

	friend class LightAssetHandler;


public:

	DirectionalLight();
	DirectionalLight(const Vector3f aColor, const Vector3f aRotation, float aIntensity);


	void Update() override;

	void SetAsResource(Microsoft::WRL::ComPtr<ID3D11Buffer> aLightBuffer, unsigned int aShaderIndex) override;
	void SetData(TransformComponent* aTransform) override;
};
