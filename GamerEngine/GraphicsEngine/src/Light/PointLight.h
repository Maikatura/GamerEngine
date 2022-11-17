#pragma once
#include "Light.h"


class PointLight : public Light
{
	friend class LightAssetHandler;

public:
	virtual ~PointLight() override = default;


	void SetAsResource(Microsoft::WRL::ComPtr<ID3D11Buffer> aLightBuffer) override;

	void Update() override;

	void SetRange(float aRange);

	void SetData(TransformComponent* aTransform) override;

	void CreatePointLightMap(Vector2f aResolution);

	FORCEINLINE float GetRange() const
	{
		return myLightBufferData.Range;
	}
};
