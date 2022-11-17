#include "GraphicsEngine.pch.h"
#include "Light/PointLight.h"

#include "Components/Components.hpp"

void PointLight::SetAsResource(Microsoft::WRL::ComPtr<ID3D11Buffer> aLightBuffer)
{
	if(myLightBufferData.CastShadows)
	{
		DX11::Context->PSSetShaderResources(myLightBufferData.ShadowMapIndex, 1, myShadowMap->mySRV.GetAddressOf());
	}
}

void PointLight::Update()
{
	Matrix4x4f wTrans = Matrix4x4f::BuildTransform(myTransform->Translation, {0,0,0}, {1,1,1});
	SetDirection(myTransform->Rotation);
	SetLightPosition(myTransform->Translation);


	// Ignore these magic Numbers
	myLightBufferData.LightView = Matrix4x4f::GetFastInverse(Matrix4x4f::CreateRotationAroundY(3.14159f * .5f) * wTrans);
	myLightBufferData.LightView1 = Matrix4x4f::GetFastInverse(Matrix4x4f::CreateRotationAroundY(3.14159f * 1.5f) * wTrans);
	myLightBufferData.LightView2 = Matrix4x4f::GetFastInverse(Matrix4x4f::CreateRotationAroundX(3.14159f * -.5f) * wTrans);
	myLightBufferData.LightView3 = Matrix4x4f::GetFastInverse(Matrix4x4f::CreateRotationAroundX(3.14159f * .5f) * wTrans);
	myLightBufferData.LightView4 = Matrix4x4f::GetFastInverse(wTrans);
	myLightBufferData.LightView5 = Matrix4x4f::GetFastInverse(Matrix4x4f::CreateRotationAroundY(3.14159f) * wTrans);
	myLightBufferData.ShadowMapIndex = 40 + mySpotAndDirLightInt;
	mySpotAndDirLightInt++;
}

void PointLight::SetRange(float aRange)
{
	myLightBufferData.Range = aRange;
}

void PointLight::SetData(TransformComponent* aTransform)
{
	myTransform = aTransform;
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
