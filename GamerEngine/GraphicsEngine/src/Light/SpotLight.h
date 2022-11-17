#pragma once
#include "Light.h"


class SpotLight : public Light
{
	friend class LightAssetHandler;

	TransformComponent* myTransform;

public:
	virtual ~SpotLight() override = default;


	void SetAsResource(Microsoft::WRL::ComPtr<ID3D11Buffer> aLightBuffer) override;

	void SetRange(float aRange);
	void SetInnerCone(float aRadius);
	void SetOuterCone(float aRadius);


	void Update() override;

	void SetData(TransformComponent* data) override;

	FORCEINLINE float GetRange() const
	{
		return myLightBufferData.Range;
	}

	FORCEINLINE float GetInnerCone() const
	{
		return myLightBufferData.SpotInnerRadius;
	}

	FORCEINLINE float GetOuterCone() const
	{
		return myLightBufferData.SpotOuterRadius;
	}
};
