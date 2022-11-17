#include "GraphicsEngine.pch.h"
#include <Light/DirectionalLight.h>

#include "Framework/DX11.h"
#include "Render/Renderer.h"
#include <Components/Components.hpp>

DirectionalLight::DirectionalLight()
{
	
}

void DirectionalLight::Update()
{
	SetActive(myDirLight->Active);
	SetIntensity(myDirLight->Intensity);
	SetColor(myDirLight->Color);

	Matrix4x4f cTran = Renderer::GetViewMatrix();
	Vector4f lPos = cTran.GetPositionW() + (cTran.GetForwardW() * 1000);
	Matrix4x4f wTran = myTransform->GetMatrix();
	myLightBufferData.Direction = wTran.GetForward();

	Vector4f wPos = (Vector4f(0, 0, 1, 0) * -10000);
	wPos.w = 1;
	wPos *= wTran;
	wPos += lPos;
	wTran(4, 1) = wPos.x;
	wTran(4, 2) = wPos.y;
	wTran(4, 3) = wPos.z;
	wTran(4, 4) = 1;
	myLightBufferData.Position = wTran.GetPosition();
	myLightBufferData.LightView = Matrix4x4f::GetFastInverse(wTran);

	myLightBufferData.ShadowMapIndex = 19;
}

void DirectionalLight::SetAsResource(Microsoft::WRL::ComPtr<ID3D11Buffer> aLightBuffer)
{
	aLightBuffer;
}

void DirectionalLight::SetAsResource()
{
	if(myLightBufferData.CastShadows)
	{
		DX11::Context->PSSetShaderResources(19, 1, myShadowMap->mySRV.GetAddressOf());
	}
}

void DirectionalLight::SetData(TransformComponent* aTransform, DirectionalLightComponent* dirLight)
{
	myTransform = aTransform;
	myDirLight = dirLight;

	
}




