#include "GraphicsEngine.pch.h"
#include <Light/SpotLight.h>

#include "AssetHandlers/TextureAssetHandler.h"
#include "Debugger/ConsoleHelper.h"
#include "Framework/DX11.h"
#include "Render/Renderer.h"

void SpotLight::SetAsResource(Microsoft::WRL::ComPtr<ID3D11Buffer> aLightBuffer)
{
	if(myLightBufferData.CastShadows)
	{
		DX11::Context->PSSetShaderResources(myLightBufferData.ShadowMapIndex, 1, myShadowMap->mySRV.GetAddressOf());
	}
}


void SpotLight::SetRange(float aRange)
{
	myLightBufferData.Range = aRange;
}

void SpotLight::SetInnerCone(float aRadius)
{
	myLightBufferData.SpotInnerRadius = aRadius;
}

void SpotLight::SetOuterCone(float aRadius)
{
	myLightBufferData.SpotOuterRadius = aRadius;
}

void SpotLight::Update()
{
	Matrix4x4f wTrans = myTransform->GetMatrix();
	SetDirection(myTransform->Rotation);
	SetLightPosition(myTransform->Translation);
	myLightBufferData.LightView = Matrix4x4f::GetFastInverse(wTrans);
	myLightBufferData.ShadowMapIndex = 20 + mySpotAndDirLightInt;
	mySpotAndDirLightInt++;
}

void SpotLight::SetData(TransformComponent* aTransform)
{
	myTransform = aTransform;
}
