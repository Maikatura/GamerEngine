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
		DX11::Get().GetContext()->PSSetShaderResources(20 + myLightData.ShadowMapIndex, 1, myShadowMap->mySRV.GetAddressOf());
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
	SetLightPosition(myTransform->GetPosition());

	// Get the rotation quaternion
	Vector3f rotation = myTransform->GetRotation();
	CommonUtilities::Quat rotationQuaternion = CommonUtilities::Quat::FromEulers(ToRadians(Vector3f(rotation.x, rotation.y, rotation.z)));

	// Set the direction using the rotation quaternion
	SetDirection(rotationQuaternion * Vector3f(0.0f, 0.0f, 1.0f));

	myLightData.CastShadows = myCastShadows;


	// Set the LightView using the inverse of the transformation matrix
	myLightData.LightView[0] = Matrix4x4f::GetFastInverse(wTrans);

	//Matrix4x4f wTrans = myTransform->GetMatrix();
	//SetDirection(myTransform->GetRotation());
	//SetLightPosition(myTransform->GetPosition());
	//myLightData.LightView[0] = Matrix4x4f::GetFastInverse(wTrans);
	myLightData.ShadowMapIndex = SpotLightNum;
	SpotLightNum++;
}

void SpotLight::SetData(TransformComponent* aTransform)
{
	myTransform = aTransform;
}
