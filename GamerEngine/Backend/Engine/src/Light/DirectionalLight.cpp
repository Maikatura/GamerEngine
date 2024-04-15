#include "GraphicsEngine.pch.h"
#include <Light/DirectionalLight.h>
#include "Core/Framework/DX11.h"
#include "Core/Rendering/Renderer.h"
#include "Core/Rendering/RendererBase.h"
#include "Components/TransfromComponent.h"



DirectionalLight::DirectionalLight()
{
	SetLightDirection({ 0,0,0 });
}

DirectionalLight::DirectionalLight(const Vector3f aColor, const Vector3f aRotation, float aIntensity)
{
	SetIntensity(aIntensity);
	SetColor(aColor);
	SetLightDirection(aRotation);
}

void DirectionalLight::Update()
{
	myLightData.NearPlane = myNearPlane;
	myLightData.FarPlane = myFarPlane;

	// Set the direction using the rotation quaternion
	//myTransformComp->GetRotation().z = 0.0f;
	//SetLightPosition(myTransformComp->GetPosition() - (myLightData.Direction * 3000.0f));

	if (!myTransformComp)
	{
		return;
	}


	if (!Renderer::GetCamera())
	{
		return;
	}

	Vector3f rotation = myTransformComp->GetRotation();
	Matrix4x4f viewMat = ComposeFromTRS(myLightData.Position, Vector3f(rotation.x, rotation.y, rotation.z), { 1, 1, 1 });

	//SetLightDirection(rotationQuaternion);
	myLightData.Direction = viewMat.GetForward();
	SetLightPosition(Renderer::GetCamera()->GetPosition() - myLightData.Direction * 100000.f);
	myLightData.LightView[0] = Matrix4x4f::GetFastInverse(viewMat);
	myLightData.ShadowMapIndex = 19;

}

void DirectionalLight::SetAsResource(Microsoft::WRL::ComPtr<ID3D11Buffer> aLightBuffer, unsigned int aShaderIndex)
{
	if(GetActive())
	{
		DX11::Get().GetContext()->PSSetShaderResources(19, 1, myShadowMap->mySRV.GetAddressOf());
	}
}

void DirectionalLight::SetData(TransformComponent* aTransform)
{
	myTransformComp = aTransform;
}




