#include "GraphicsEngine.pch.h"
#include <Light/SpotLight.h>

#include "AssetHandlers/TextureAssetHandler.h"
#include "Debugger/ConsoleHelper.h"
#include "Framework/DX11.h"
#include "Render/Renderer.h"
#include "Components/TransfromComponent.h"

void SpotLight::SetAsResource(Microsoft::WRL::ComPtr<ID3D11Buffer> aLightBuffer, int aShaderIndex)
{
	if(myLightData.CastShadows)
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
	// Get the rotation quaternion
	myTransform->GetRotation().z = 0.0f;
	Vector3f rotation = myTransform->GetRotation();
	CommonUtilities::Quat rotationQuaternion = CommonUtilities::Quat::FromEulers(ToRadians(Vector3f(rotation.x, rotation.y, rotation.z)));

	// Set the direction using the rotation quaternion
	SetLightDirection(rotationQuaternion);
	SetLightPosition(myTransform->GetPosition());

	myLightData.LightView[0] = Matrix4x4f::GetFastInverse(ComposeFromTRS(myLightData.Position, rotationQuaternion, { 1, 1, 1 }));

	myLightData.ShadowMapIndex = SpotLightNum;
	SpotLightNum++;
}

void SpotLight::SetData(TransformComponent* aTransform)
{
	myTransform = aTransform;
}
