#include "GraphicsEngine.pch.h"
#include "Light/PointLight.h"

#include "Components/Components.hpp"
#include "Math/Quaternion.hpp"
#include "Framework/DX11.h"
#include "Components/TransfromComponent.h"

void PointLight::SetAsResource(Microsoft::WRL::ComPtr<ID3D11Buffer> aLightBuffer, int aShaderIndex)
{
	if(myLightData.CastShadows)
	{
		DX11::Get().GetContext()->PSSetShaderResources(40 + aShaderIndex, 1, myShadowMap->mySRV.GetAddressOf());
	}
}

void PointLight::Update()
{
	SetLightDirection({0,0,0});
	SetLightPosition(myTransformComp->GetPosition());
	//SetIntensity(myLightData.Intensity);


	myLightData.CastShadows = myCastShadows;
	
	

	myLightData.LightView[0] = Matrix4x4f::GetFastInverse(ComposeFromTRS(myLightData.Position, Vector3f{ 0, -90, 0 }, { 1, 1, 1 }));
	myLightData.LightView[1] = Matrix4x4f::GetFastInverse(ComposeFromTRS(myLightData.Position, Vector3f{ 0, 90, 0 }, { 1, 1, 1 }));
	myLightData.LightView[2] = Matrix4x4f::GetFastInverse(ComposeFromTRS(myLightData.Position, Vector3f{ 90, 0, 0 }, { 1, 1, 1 }));
	myLightData.LightView[3] = Matrix4x4f::GetFastInverse(ComposeFromTRS(myLightData.Position, Vector3f{ -90, 0, 0 }, { 1, 1, 1 }));
	myLightData.LightView[4] = Matrix4x4f::GetFastInverse(ComposeFromTRS(myLightData.Position, Vector3f{ 0, 0, 0 }, { 1, 1, 1 }));
	myLightData.LightView[5] = Matrix4x4f::GetFastInverse(ComposeFromTRS(myLightData.Position, Vector3f{ 0,  180, 0 }, { 1, 1, 1 }));

	//myLightData.LightView[0] = Matrix4x4f::GetFastInverse(ComposeFromTRS(myLightData.Position, CommonUtilities::Quat(0.7071f, 0.0f, 0.7071f, 0.0f), { 1, 1, 1 }));
	//myLightData.LightView[1] = Matrix4x4f::GetFastInverse(ComposeFromTRS(myLightData.Position, CommonUtilities::Quat(0.7071f, 0.0f, -0.7071f, 0.0f), { 1, 1, 1 }));
	//myLightData.LightView[2] = Matrix4x4f::GetFastInverse(ComposeFromTRS(myLightData.Position, CommonUtilities::Quat(0.7071f, -0.7071f, 0.0f, 0.0f), { 1, 1, 1 }));
	//myLightData.LightView[3] = Matrix4x4f::GetFastInverse(ComposeFromTRS(myLightData.Position, CommonUtilities::Quat(0.7071f, 0.7071f, 0.0f, 0.0f), { 1, 1, 1 }));
	//myLightData.LightView[4] = Matrix4x4f::GetFastInverse(ComposeFromTRS(myLightData.Position, CommonUtilities::Quat(1.0f, 0.0f, 0.0f, 0.0f), { 1, 1, 1 }));
	//myLightData.LightView[5] = Matrix4x4f::GetFastInverse(ComposeFromTRS(myLightData.Position, CommonUtilities::Quat(0.0f, 0.0f, 1.0f, 0.0f), { 1.0f, 1.0f, 1.0f }));

	myLightData.Direction =	(ComposeFromTRS(myLightData.Position, Vector3f{ 0, 0, 0 }, { 1, 1, 1 })).GetForward();

	myLightData.ShadowMapIndex = PointLightNum;
	PointLightNum++;
}


void PointLight::SetData(TransformComponent* aTransform)
{
	myTransformComp = aTransform;
	myCamera.SetPerspective(90, { 2048, 2048 }, myLightData.NearPlane, myLightData.FarPlane);
}


