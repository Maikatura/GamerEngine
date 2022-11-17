#pragma once
#include <Light/Light.h>

class TransformComponent;
class DirectionalLightComponent;

class DirectionalLight : public Light
{

	friend class LightAssetHandler;

	TransformComponent* myTransform;
	DirectionalLightComponent* myDirLight;


public:

	DirectionalLight();

	void Update() override;

	void SetAsResource(Microsoft::WRL::ComPtr<ID3D11Buffer> aLightBuffer) override;
	void SetAsResource();
	void SetData(TransformComponent* aTransform, DirectionalLightComponent* dirLight) override;
};
