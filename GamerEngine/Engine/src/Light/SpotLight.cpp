#include "GraphicsEngine.pch.h"
#include <Light/SpotLight.h>

#include "AssetHandlers/TextureAssetHandler.h"
#include "Debugger/ConsoleHelper.h"
#include "Framework/DX11.h"
#include "Render/Renderer.h"
#include "Components/TransfromComponent.h"

void SpotLight::SetAsResource(Microsoft::WRL::ComPtr<ID3D11Buffer> aLightBuffer, int aShaderIndex)
{
	if(GetActive())
	{
		DX11::Get().GetContext()->PSSetShaderResources(20 + aShaderIndex, 1, myShadowMap->mySRV.GetAddressOf());
	}
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

	
	SetLightPosition(myTransform->GetPosition());
	// Get the rotation quaternion
	Vector3f rotation = myTransform->GetRotation();
	Matrix4x4f viewMat = ComposeFromTRS(myLightData.Position, Vector3f(rotation.x, rotation.y, rotation.z), { 1, 1, 1 });

	SetLightDirection(viewMat.GetForward());

	myLightData.LightView[0] = Matrix4x4f::GetFastInverse(viewMat);

	myLightData.ShadowMapIndex = SpotLightNum;
	SpotLightNum++;
}

void SpotLight::SetData(TransformComponent* aTransform)
{
	myTransform = aTransform;
}
