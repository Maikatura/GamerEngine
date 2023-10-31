#include "GraphicsEngine.pch.h"
#include <Light/SpotLight.h>

#include "AssetHandlers/TextureAssetHandler.h"
#include "Debugger/ConsoleHelper.h"
#include "Framework/DX11.h"
#include "Render/Renderer.h"
#include "Components/TransfromComponent.h"

void SpotLight::SetAsResource(Microsoft::WRL::ComPtr<ID3D11Buffer> aLightBuffer)
{
	if(myLightData.CastShadows)
	{
		DX11::Get().GetContext()->PSSetShaderResources(myLightData.ShadowMapIndex, 1, myShadowMap->mySRV.GetAddressOf());
	}
}


void SpotLight::SetRange(float aRange)
{
	myLightData.Range = aRange;
}

void SpotLight::SetInnerCone(float aRadius)
{
	myLightData.SpotInnerRadius = aRadius;
}

void SpotLight::SetOuterCone(float aRadius)
{
	myLightData.SpotOuterRadius = aRadius;
}

void SpotLight::Update()
{
	Matrix4x4f wTrans = myTransform->GetMatrix();
	SetDirection(myTransform->GetRotation());
	SetLightPosition(myTransform->GetPosition());
	myLightData.LightView[0] = Matrix4x4f::GetFastInverse(wTrans);
	myLightData.ShadowMapIndex = 20 + mySpotAndDirLightInt;
	mySpotAndDirLightInt++;
}

void SpotLight::SetData(TransformComponent* aTransform)
{
	myTransform = aTransform;
}
