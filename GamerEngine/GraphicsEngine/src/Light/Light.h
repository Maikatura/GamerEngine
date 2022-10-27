#pragma once
#include <wrl/client.h>

#include "Model/SceneObject.h"
#include <d3d11.h>


class Light : public SceneObject
{
	friend class LightAssetHandler;

public:
	struct LightBufferData
	{
		Vector3f Color;
		float Intensity;
		Vector3f Direction;
		float Padding;
	};

protected:
	LightBufferData myLightBufferData;

	Vector3f myLocalDirection;

public:
	virtual ~Light() override = default;
	virtual void Init(Vector3f aColor, float anIntensity);

	virtual void SetAsResource(Microsoft::WRL::ComPtr<ID3D11Buffer> aLightBuffer) = 0;

	FORCEINLINE void SetColor(Vector3f aColor)
	{
		myLightBufferData.Color = aColor;
	}
	FORCEINLINE Vector4f GetColor() const
	{
		return { myLightBufferData.Color.x,myLightBufferData.Color.y, myLightBufferData.Color.z, 1.0f };
	}

	FORCEINLINE void SetIntensity(float aIntensity)
	{
		myLightBufferData.Intensity = aIntensity;
	}
	FORCEINLINE float GetIntensity() const
	{
		return myLightBufferData.Intensity;
	}

	FORCEINLINE void SetDirection(Vector3f aDirection)
	{
		myLocalDirection = aDirection;

		Vector3f radVector3 = ToRadians(aDirection);

		Matrix4x4f lightRotation =
			Matrix4x4f::CreateRotationAroundX(radVector3.x) *
			Matrix4x4f::CreateRotationAroundY(radVector3.y) *
			Matrix4x4f::CreateRotationAroundZ(radVector3.z);

		myLightBufferData.Direction = lightRotation.GetForward();
	}
	FORCEINLINE Vector3f GetDirection()
	{
		return myLocalDirection;
	}
};

inline void Light::Init(Vector3f aColor, float anIntensity)
{
	myLightBufferData.Color = aColor;
	myLightBufferData.Intensity = anIntensity;
}
