#include "GraphicsEngine.pch.h"
#include <Light/DirectionalLight.h>

#include "Framework/DX11.h"
#include "Render/Renderer.h"
#include <Components/Components.hpp>

#include "Math/Quaternion.hpp"
#include "Render/RendererBase.h"
#include "Components/TransfromComponent.h"
#include "ImGui/imgui.h"


DirectionalLight::DirectionalLight()
{
	SetLightDirection(CommonUtilities::Quat::FromEulers({ 0,0,0 }));
	myDirLight = nullptr;
}

DirectionalLight::DirectionalLight(const Vector3f aColor, const Vector3f aRotation, float aIntensity)
{
	SetIntensity(aIntensity);
	SetColor(aColor);
	SetLightDirection(CommonUtilities::Quat::FromEulers(aRotation));
}

void DirectionalLight::Update()
{
	if (!myDirLight)
	{
		return;
	}

	SetActive(myDirLight->Active);
	SetIntensity(myDirLight->Intensity);
	SetColor(myDirLight->Color);
	SetCastShadows(myDirLight->CastShadow);


	myLightData.NearPlane = myNearPlane;
	myLightData.FarPlane = myFarPlane;

	myLightData.SmoothShadows = myDirLight->SmoothShadows;

	/*if (!Renderer::GetCamera())
	{
		return;
	}*/

	
	// Set the direction using the rotation quaternion
	//myTransformComp->GetRotation().z = 0.0f;
	Vector3f rotation = myTransformComp->GetRotation();
	CommonUtilities::Quat rotationQuaternion = CommonUtilities::Quat::FromEulers(ToRadians(Vector3f(rotation.x, rotation.y, rotation.z)));




	//SetLightPosition(myTransformComp->GetPosition() - (myLightData.Direction * 3000.0f));
	SetLightDirection(rotationQuaternion);

	SetLightPosition(Renderer::GetCamera()->GetPosition() + myLightData.Direction * -1000.f);
	myLightData.LightView[0] = Matrix4x4f::AffineInverse(ComposeFromTRS(myLightData.Position, rotationQuaternion, { 1, 1, 1 }));
	
	myLightData.ShadowMapIndex = 19;
}

void DirectionalLight::SetAsResource(Microsoft::WRL::ComPtr<ID3D11Buffer> aLightBuffer, int aShaderIndex)
{
	if(myLightData.CastShadows)
	{
	/*	ID3D11ShaderResourceView* nullsrv = nullptr;
		DX11::Get().GetContext()->PSSetShaderResources(19, 1, &nullsrv);*/
		//DX11::Get().GetContext()->VSSetShaderResources(19, 1, &nullsrv);

		//DX11::Get().GetContext()->VSSetShaderResources(19, 1, myShadowMap->mySRV.GetAddressOf());
		DX11::Get().GetContext()->PSSetShaderResources(19, 1, myShadowMap->mySRV.GetAddressOf());
	}
}

void DirectionalLight::SetData(TransformComponent* aTransform, DirectionalLightComponent* dirLight)
{
	myTransformComp = aTransform;
	myDirLight = dirLight;

	

	//myTransform->Translation = { 10000.0f, 10000.0f , 10000.0f };
}




