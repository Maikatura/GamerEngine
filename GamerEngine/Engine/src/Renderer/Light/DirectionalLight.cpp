#include "GraphicsEngine.pch.h"
#include <Renderer/Light/DirectionalLight.h>

#include "Renderer/Framework/DX11.h"
#include "Renderer/Render/Renderer.h"
#include <Components/Components.hpp>

#include "Math/Quaternion.hpp"
#include "Renderer/Render/RendererBase.h"


DirectionalLight::DirectionalLight()
{
	SetDirection({ 0,0,0 });
	myDirLight = nullptr;
}

DirectionalLight::DirectionalLight(const Vector3f aColor, const Vector3f aRotation, float aIntensity)
{
	SetIntensity(myDirLight->Intensity);
	SetColor(myDirLight->Color);
	SetDirection(aRotation);
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

	myLightData.NearPlane = myNearPlane;
	myLightData.FarPlane = myFarPlane;

	myLightData.SmoothShadows = myDirLight->SmoothShadows;





	auto quatRot = CommonUtilities::Quaternionf::FromEulers(myTransformComp->GetRotation());
	auto matrix = ComposeFromTRS(myTransformComp->GetPosition(), quatRot, { 1, 1, 1 });

	myLightData.Direction = quatRot.Forward();
	myLightData.Position = Renderer::GetCamera()->GetPosition() + (myLightData.Direction * -3000.f);

	myLightData.LightView[0] = Matrix4x4f::GetFastInverse(matrix);
	myLightData.ShadowMapIndex = 19;
}

void DirectionalLight::SetAsResource(Microsoft::WRL::ComPtr<ID3D11Buffer> aLightBuffer)
{
	if(myLightData.CastShadows)
	{
	/*	ID3D11ShaderResourceView* nullsrv = nullptr;
		DX11::GetContext()->PSSetShaderResources(19, 1, &nullsrv);*/
		//DX11::GetContext()->VSSetShaderResources(19, 1, &nullsrv);

		DX11::GetContext()->PSSetShaderResources(19, 1, myShadowMap->mySRV.GetAddressOf());
	}
}

void DirectionalLight::SetData(TransformComponent* aTransform, DirectionalLightComponent* dirLight)
{
	myTransformComp = aTransform;
	myDirLight = dirLight;

	

	//myTransform->Translation = { 10000.0f, 10000.0f , 10000.0f };
}




