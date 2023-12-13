#pragma once
#include <wrl/client.h>

#include "Model/SceneObject.h"
#include <d3d11.h>


#include "Framework/DX11.h"
#include "Math/Quaternion.hpp"
#include "Render/DepthStencil.h"
#include "Render/RenderTexture.h"

class TransformComponent;
class DirectionalLightComponent;

class Light : public SceneObject
{
	friend class LightAssetHandler;

	bool myIsActive = true;

public:
	struct LightBufferData
	{
		Matrix4x4f LightView[6];

		Matrix4x4f LightProjection; // 64 Byte

		Vector3f Color;
		float Intensity;		// 16 Byte

		Vector3f Direction;
		float Range;			// 16 Byte

		Vector3f Position;
		float Attenuation;		// 16 Byte

		float SpotInnerRadius;
		float SpotOuterRadius;
		unsigned int LightType;
		alignas(4) bool CastShadows; // 16 Byte

		float NearPlane;
		float FarPlane;
		unsigned int ShadowMapIndex;
		float VogalDistScale;

		alignas(4) bool SmoothShadows;
		Vector3f Padding;
	};

protected:
	LightBufferData myLightData;

	float myNearPlane = 0.0f;
	float myFarPlane = 0.0f;
	bool myCastShadows = true;

	Ref<DepthStencil> myShadowMap;
	Ref<RenderTexture> myWorldPositionMap;
	Ref<RenderTexture> myWorldNormalMap;
	Ref<RenderTexture> myFluxMap;

	Vector3f myLocalDirection;

	TransformComponent* myTransformComp;

	inline static int SpotLightNum = 0;
	inline static int PointLightNum = 0;

public:


	virtual void Update() {}

	static void Reset()
	{
		SpotLightNum = 0;
		PointLightNum = 0;
	}

	bool operator<(const Light& aLight) const
	{
		return (myLightData.ShadowMapIndex < aLight.GetLightBufferData().ShadowMapIndex);
	}

	virtual ~Light() override = default;
	virtual void Init(Vector3f aColor, float anIntensity)
	{
		myLightData.Color = aColor;
		myLightData.Intensity = anIntensity;

		

		
	}

	virtual void SetAsResource(Microsoft::WRL::ComPtr<ID3D11Buffer> aLightBuffer, int aShaderIndex) = 0;

	FORCEINLINE void SetColor(Vector3f aColor)
	{
		myLightData.Color = aColor;
	}
	FORCEINLINE Vector4f GetColor() const
	{
		return { myLightData.Color.x,myLightData.Color.y, myLightData.Color.z, 1.0f };
	}

	FORCEINLINE void SetIntensity(float aIntensity)
	{
		myLightData.Intensity = aIntensity;
	}
	FORCEINLINE float GetIntensity() const
	{
		return myLightData.Intensity;
	}

	FORCEINLINE void SetDirection(Vector3f aDirection)
	{
		myLocalDirection = aDirection;
		myLightData.Direction = aDirection;
	}
	FORCEINLINE Vector3f GetDirection()
	{
		return myLocalDirection;
	}

	FORCEINLINE void SetLightPosition(Vector3f aPosition)
	{
		myLightData.Position = aPosition;
	}

	FORCEINLINE LightBufferData GetLightBufferData() const { return myLightData; }

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

	void SetCastShadows(bool aCastShadows)
	{
		myCastShadows = aCastShadows;
	}

	bool HasShadowMap()
	{
		return (myShadowMap != nullptr);
	}

	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> GetShadowDSV()
	{
		return myShadowMap->myDSV;
	}

	inline static ID3D11DepthStencilState* pShadowDepthStencilState = nullptr;

	void ClearShadowMap()
	{

		if (!pShadowDepthStencilState)
		{
			D3D11_DEPTH_STENCIL_DESC dsDesc = {};
			dsDesc.DepthEnable = TRUE; // Enable depth testing
			dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL; // Allow writing to the depth buffer
			dsDesc.DepthFunc = D3D11_COMPARISON_LESS; // Depth test function

			DX11::Get().GetDevice()->CreateDepthStencilState(&dsDesc, &pShadowDepthStencilState);
		}

		if (HasShadowMap())
		{
			if (myShadowMap->myDSV)
			{
				//DX11::Get().GetContext()->ClearDepthStencilView(myShadowMap->myDSV.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
				//DX11::Get().GetContext()->OMSetRenderTargets(0, nullptr, nullptr);
			}
		}
	}

	void SetShadowMapAsDepth()
	{
		if(HasShadowMap())
		{
			if(myShadowMap->myDSV)
			{
				DX11::Get().GetContext()->ClearDepthStencilView(myShadowMap->myDSV.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
				DX11::Get().GetContext()->OMSetDepthStencilState(pShadowDepthStencilState, 0);


				DX11::Get().GetContext()->RSSetViewports(1, &myShadowMap->myViewport);
				DX11::Get().GetContext()->OMSetRenderTargets(0, nullptr, myShadowMap->myDSV.Get());
			}
		}
	}

	virtual void SetData(TransformComponent* aTransform) { aTransform; }
	virtual void SetData(TransformComponent* aTransform, DirectionalLightComponent* dirLight) { aTransform; dirLight; }
};
