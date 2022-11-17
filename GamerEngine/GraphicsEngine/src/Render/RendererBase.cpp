#include "GraphicsEngine.pch.h"
#include "Render/RendererBase.h"

#include "Framework/DX11.h"

bool RendererBase::Init()
{
	HRESULT result;

	D3D11_SAMPLER_DESC samplerDesc;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0.0f;
	samplerDesc.BorderColor[1] = 0.0f;
	samplerDesc.BorderColor[2] = 0.0f;
	samplerDesc.BorderColor[3] = 0.0f;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	result = DX11::Device->CreateSamplerState(&samplerDesc, mySamplerStates[(int)SamplerState::SS_Default].GetAddressOf());
	DX11::Context->PSSetSamplers(0, 1, mySamplerStates[(int)SamplerState::SS_Default].GetAddressOf());

	D3D11_SAMPLER_DESC pointSampleDesc;

	pointSampleDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	pointSampleDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	pointSampleDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	pointSampleDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	pointSampleDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	pointSampleDesc.MaxAnisotropy = 1;
	pointSampleDesc.MipLODBias = 1.0f;
	pointSampleDesc.BorderColor[0] = 0.0f;
	pointSampleDesc.BorderColor[1] = 0.0f;
	pointSampleDesc.BorderColor[2] = 0.0f;
	pointSampleDesc.BorderColor[3] = 0.0f;
	pointSampleDesc.MinLOD = -FLT_MAX;
	pointSampleDesc.MaxLOD = FLT_MAX;
	result = DX11::Device->CreateSamplerState(&pointSampleDesc, mySamplerStates[(int)SamplerState::SS_PointClamp].GetAddressOf());
	DX11::Context->PSSetSamplers(1, 1, mySamplerStates[(int)SamplerState::SS_PointClamp].GetAddressOf());

	
	myBlendStates[(int)BlendState::AdditiveBlend] = nullptr;

	D3D11_BLEND_DESC blendDesc = {};
	blendDesc.RenderTarget[0].BlendEnable = true;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_MAX;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	result = DX11::Device->CreateBlendState(&blendDesc, &myBlendStates[(int)BlendState::AlphaBlend]);

	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	result = DX11::Device->CreateBlendState(&blendDesc, &myBlendStates[(int)BlendState::AdditiveBlend]);

	myDepthStencilStates[(int)DepthStencilState::DSS_ReadWrite] = nullptr;

	D3D11_DEPTH_STENCIL_DESC depthDesc = {};
	depthDesc.DepthEnable = true;
	depthDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	depthDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthDesc.StencilEnable = false;
	result = DX11::Device->CreateDepthStencilState(&depthDesc, &myDepthStencilStates[(int)DepthStencilState::DSS_ReadOnly]);


	depthDesc.DepthEnable = false;
	depthDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	depthDesc.DepthFunc = D3D11_COMPARISON_NEVER;
	result = DX11::Device->CreateDepthStencilState(&depthDesc, &myDepthStencilStates[(int)DepthStencilState::DSS_Ignore]);


	D3D11_DEPTH_STENCILOP_DESC depthOPDesc = {};
	depthDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthDesc.DepthFunc = D3D11_COMPARISON_ALWAYS;
	depthOPDesc.StencilPassOp = D3D11_STENCIL_OP_INCR;
	//depthDesc.StencilEnable = true;
	depthDesc.FrontFace = depthOPDesc;
	result = DX11::Device->CreateDepthStencilState(&depthDesc, &myDepthStencilStates[(int)DepthStencilState::DSS_Overlay]);


	return true;
}

void RendererBase::SetDepthStencilState(DepthStencilState aDepthStencilState)
{
	DX11::Context->OMSetDepthStencilState(myDepthStencilStates[(int)aDepthStencilState].Get(), 0xffffffff);
}

void RendererBase::SetBlendState(BlendState aBlendState)
{
	DX11::Context->OMSetBlendState(myBlendStates[(int)aBlendState].Get(), nullptr, 0xffffffff);
}

void RendererBase::SetSamplerState(SamplerState aSamplerState, int aStartSlot) 
{
	DX11::Context->PSSetSamplers(aStartSlot, 1, mySamplerStates[(int)aSamplerState].GetAddressOf());
}
