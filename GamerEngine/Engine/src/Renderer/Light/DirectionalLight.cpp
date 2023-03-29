#include "GraphicsEngine.pch.h"
#include <Renderer/Light/DirectionalLight.h>

#include "Renderer/Framework/DX11.h"
#include "Renderer/Render/Renderer.h"
#include <Components/Components.hpp>

#include "Math/Quaternion.hpp"
#include "Renderer/Render/RendererBase.h"


inline Matrix4x4f ComposeFromTRS(const Vector3f& aTranslation, const CommonUtilities::Quaternion<float>& aRotationQuat, const Vector3f& aScale)
{
	return Matrix4x4f::CreateScale(aScale)
		* aRotationQuat.GetRotationMatrix4x4()
		* Matrix4x4f::CreateTranslation(aTranslation);
}

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

	Vector3f rotation = { myTransformComp->Rotation.x, myTransformComp->Rotation.y, myTransformComp->Rotation.z };
	CommonUtilities::Quat rotation2 = CommonUtilities::Quat::FromEulers(rotation * DEG_TO_RAD);
	myLocalDirection = rotation;
	myLightData.Direction = rotation2.Forward();
	myLightData.Position = Renderer::GetCamera()->GetPosition() + (myLightData.Direction * -3000.f);

	auto matrix = ComposeFromTRS(myLightData.Position, rotation2, { 1, 1, 1 });
	myLightData.LightView[0] = Matrix4x4f::GetFastInverse(matrix);
	myLightData.ShadowMapIndex = 19;
}

void DirectionalLight::SetAsResource(Microsoft::WRL::ComPtr<ID3D11Buffer> aLightBuffer)
{
	if(myLightData.CastShadows)
	{
	/*	ID3D11ShaderResourceView* nullsrv = nullptr;
		DX11::Context->PSSetShaderResources(19, 1, &nullsrv);*/
		//DX11::Context->VSSetShaderResources(19, 1, &nullsrv);

		DX11::Context->PSSetShaderResources(19, 1, myShadowMap->mySRV.GetAddressOf());
		//DX11::Context->VSSetShaderResources(19, 1, myShadowMap->mySRV.GetAddressOf());
	}
}

void DirectionalLight::SetData(TransformComponent* aTransform, DirectionalLightComponent* dirLight)
{
	myTransformComp = aTransform;
	myDirLight = dirLight;

	

	//myTransform->Translation = { 10000.0f, 10000.0f , 10000.0f };
}




