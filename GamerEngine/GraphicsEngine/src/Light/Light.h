#pragma once
#include <wrl/client.h>

#include "Model/SceneObject.h"
#include <d3d11.h>


#include "Framework/DX11.h"
#include "Render/DepthStencil.h"

class TransformComponent;
class DirectionalLightComponent;

class Light : public SceneObject
{
	friend class LightAssetHandler;

	bool myIsActive = true;

public:
	struct LightBufferData
	{
		Matrix4x4f LightView;
		Matrix4x4f LightView1;
		Matrix4x4f LightView2;
		Matrix4x4f LightView3;
		Matrix4x4f LightView4;
		Matrix4x4f LightView5;

		Matrix4x4f LightProjection; // 64 Byte

		Vector3f Color;
		float Intensity;		// 16 Byte

		Vector3f Direction;
		float Range;			// 16 Byte

		Vector3f Position;
		float Attenuation;		// 16 Byte

		float SpotInnerRadius;
		float SpotOuterRadius;
		int LightType;
		alignas(4) bool CastShadows; // 16 Byte

		float NearPlane;
		float FarPlane;
		Vector2f Padding_1;			// 16 Byte

		int ShadowMapIndex;
		Vector3f Padding_2;
	};

protected:
	LightBufferData myLightBufferData;
	std::unique_ptr<DepthStencil> myShadowMap;

	Vector3f myLocalDirection;

	TransformComponent* myTransform;

	inline static int mySpotAndDirLightInt = 0;

public:


	virtual void Update() {}

	static void Reset()
	{
		mySpotAndDirLightInt = 0;
	}

	bool operator<(const Light& aLight) const
	{
		return (myLightBufferData.ShadowMapIndex < aLight.GetLightBufferData().ShadowMapIndex);
	}

	virtual ~Light() override = default;
	virtual void Init(Vector3f aColor, float anIntensity)
	{
		myLightBufferData.Color = aColor;
		myLightBufferData.Intensity = anIntensity;
	}

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

	FORCEINLINE void SetLightPosition(Vector3f aPosition)
	{
		myLightBufferData.Position = aPosition;
	}

	FORCEINLINE LightBufferData GetLightBufferData() const { return myLightBufferData; }

	ID3D11ShaderResourceView* GetShadowMapView()
	{
		return myShadowMap->mySRV.Get();
	}

	D3D11_VIEWPORT& GetViewport()
	{
		return myShadowMap->myViewport;
	}

	bool GetActive()
	{
		return myIsActive;
	}

	void SetActive(bool aActive)
	{
		myIsActive = aActive;
	}

	bool HasShadowMap()
	{
		return (myShadowMap != nullptr);
	}

	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> GetShadowDSV()
	{
		return myShadowMap->myDSV;
	}

	void ClearShadowMap()
	{
		if (HasShadowMap())
		{
			if (myShadowMap->myDSV)
			{
				DX11::Context->ClearDepthStencilView(myShadowMap->myDSV.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
			}
		}
	}

	void SetShadowMapAsDepth()
	{
		if(HasShadowMap())
		{
			if(myShadowMap->myDSV)
			{
				DX11::Context->OMSetRenderTargets(0, nullptr, myShadowMap->myDSV.Get());
			}
		}
	}

	virtual void SetData(TransformComponent* aTransform) {}
	virtual void SetData(TransformComponent* aTransform, DirectionalLightComponent* dirLight) {}
};
