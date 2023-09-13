#include "GraphicsEngine.pch.h"
#include "Renderer/Light/PointLight.h"

#include "Components/Components.hpp"
#include "Math/Quaternion.hpp"
#include "Renderer/Framework/DX11.h"

#define DEG_TO_RAD 0.0174532925f

inline Matrix4x4f ComposeFromTRS(const Vector3f& aTranslation, const CommonUtilities::Quaternion<float>& aRotationQuat, const Vector3f& aScale)
{
	return Matrix4x4f::CreateScale(aScale)
		* aRotationQuat.GetRotationMatrix4x4()
		* Matrix4x4f::CreateTranslation(aTranslation);
}

void PointLight::SetAsResource(Microsoft::WRL::ComPtr<ID3D11Buffer> aLightBuffer)
{
	if(myLightData.CastShadows)
	{
		DX11::GetContext()->PSSetShaderResources(myLightData.ShadowMapIndex, 1, myShadowMap->mySRV.GetAddressOf());
	}
}

void PointLight::Update()
{
	SetDirection(myTransformComp->Rotation);
	SetLightPosition(myTransformComp->Translation);
	SetIntensity(myLightData.Intensity);

	myLightData.LightView[0] = Matrix4x4f::GetFastInverse(Matrix4x4f::AffliteMatrix(ComposeFromTRS(myLightData.Position, CommonUtilities::Quat::FromEulers({ 0, DEG_TO_RAD * 90, 0 }), { 1, 1, 1 })));
	myLightData.LightView[1] = Matrix4x4f::GetFastInverse(Matrix4x4f::AffliteMatrix(ComposeFromTRS(myLightData.Position, CommonUtilities::Quat::FromEulers({ 0, DEG_TO_RAD * -90, 0 }), { 1, 1, 1 })));
	myLightData.LightView[2] = Matrix4x4f::GetFastInverse(Matrix4x4f::AffliteMatrix(ComposeFromTRS(myLightData.Position, CommonUtilities::Quat::FromEulers({ DEG_TO_RAD * -90, 0, 0 }), { 1, 1, 1 })));
	myLightData.LightView[3] = Matrix4x4f::GetFastInverse(Matrix4x4f::AffliteMatrix(ComposeFromTRS(myLightData.Position, CommonUtilities::Quat::FromEulers({ DEG_TO_RAD * 90, 0, 0 }), { 1, 1, 1 })));
	myLightData.LightView[4] = Matrix4x4f::GetFastInverse(Matrix4x4f::AffliteMatrix(ComposeFromTRS(myLightData.Position, CommonUtilities::Quat::FromEulers({ 0, 0, 0 }), { 1, 1, 1 })));
	myLightData.LightView[5] = Matrix4x4f::GetFastInverse(Matrix4x4f::AffliteMatrix(ComposeFromTRS(myLightData.Position, CommonUtilities::Quat::FromEulers({ 0, DEG_TO_RAD * 180, 0 }), { 1, 1, 1 })));
	myLightData.ShadowMapIndex = 40 + mySpotAndDirLightInt;
	mySpotAndDirLightInt++;
}

void PointLight::SetRange(float aRange)
{
	myLightData.Range = aRange;
}

void PointLight::SetData(TransformComponent* aTransform)
{
	myTransformComp = aTransform;
}

void PointLight::CreatePointLightMap(Vector2f aResolution)
{
	D3D11_TEXTURE2D_DESC desc = { 0 };
	desc.Width = static_cast<unsigned int>(aResolution.x);
	desc.Height = static_cast<unsigned int>(aResolution.y);
	desc.MipLevels = 1;
	desc.ArraySize = 6;
	desc.Format = DXGI_FORMAT_R32_TYPELESS;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;
	DX11::Device->CreateTexture2D(&desc, nullptr, reinterpret_cast<ID3D11Texture2D**>(myShadowMap->myTexture.GetAddressOf()));
	//assert(SUCCEEDED(result));

	D3D11_SHADER_RESOURCE_VIEW_DESC resourceDesc = {};
	ZeroMemory(&resourceDesc, sizeof(resourceDesc));
	resourceDesc.Format = DXGI_FORMAT_R32_FLOAT;
	resourceDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	resourceDesc.Texture2D.MipLevels = desc.MipLevels;
	DX11::Device->CreateShaderResourceView(myShadowMap->myTexture.Get(), &resourceDesc, myShadowMap->mySRV.GetAddressOf());
	//assert(SUCCEEDED(result));

	D3D11_DEPTH_STENCIL_VIEW_DESC depthDesc = {};
	depthDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
	depthDesc.Texture2DArray.MipSlice = 0;
	depthDesc.Texture2DArray.ArraySize = desc.ArraySize;
	depthDesc.Texture2DArray.FirstArraySlice = 0;
	DX11::Device->CreateDepthStencilView(myShadowMap->myTexture.Get(), &depthDesc, myShadowMap->myDSV.GetAddressOf());
	//assert(SUCCEEDED(result));
}
