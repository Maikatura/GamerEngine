#pragma once
#include "Light.h"


class SpotLight : public Light
{
	friend class LightAssetHandler;

	TransformComponent* myTransform;

public:
	virtual ~SpotLight() override = default;


	void SetAsResource(Microsoft::WRL::ComPtr<ID3D11Buffer> aLightBuffer,unsigned int aShaderIndex) override;

	void SetInnerCone(float aRadius);
	void SetOuterCone(float aRadius);


	void Update() override;

	void SetData(TransformComponent* data) override;

	FORCEINLINE float GetInnerCone() const
	{
		return myLightData.SpotInnerRadius;
	}

	FORCEINLINE float GetOuterCone() const
	{
		return myLightData.SpotOuterRadius;
	}
};
