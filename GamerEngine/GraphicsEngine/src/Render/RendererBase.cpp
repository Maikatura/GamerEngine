#include "GraphicsEngine.pch.h"
#include "Render/RendererBase.h"

#include "Framework/DX11.h"

bool RendererBase::Init()
{
	HRESULT result = S_FALSE;

	D3D11_BUFFER_DESC bufferDescription = { 0 };
	D3D11_SUBRESOURCE_DATA vxSubresource{};
	bufferDescription.Usage = D3D11_USAGE_DYNAMIC;
	bufferDescription.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	myInstancedTransformBufferData.resize(2000);
	vxSubresource.pSysMem = &myInstancedTransformBufferData[0];
	bufferDescription.ByteWidth = sizeof(Matrix4x4f) * 2000;
	bufferDescription.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	result = DX11::Device->CreateBuffer(&bufferDescription, &vxSubresource, myInstanceBuffer.GetAddressOf());
	if(FAILED(result))
	{
		return false;
	}


	D3D11_SAMPLER_DESC samplerDesc;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.MipLODBias = 0.f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 1.f;
	samplerDesc.BorderColor[1] = 1.f;
	samplerDesc.BorderColor[2] = 1.f;
	samplerDesc.BorderColor[3] = 1.f;
	samplerDesc.MinLOD = -D3D11_FLOAT32_MAX;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	result = DX11::Device->CreateSamplerState(&samplerDesc, mySamplerStates[(int)SamplerState::SS_Default].GetAddressOf());
	if(FAILED(result))
	{
		return false;
	}
	
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	result = DX11::Device->CreateSamplerState(&samplerDesc, mySamplerStates[(int)SamplerState::SS_Wrap].GetAddressOf());
	if(FAILED(result))
	{
		return false;
	}

	D3D11_SAMPLER_DESC pointSampleDesc = {};
	pointSampleDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	pointSampleDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	pointSampleDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	pointSampleDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	pointSampleDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	pointSampleDesc.MinLOD = -FLT_MAX;
	pointSampleDesc.MaxLOD = FLT_MAX;

	result = DX11::Device->CreateSamplerState(&pointSampleDesc, &mySamplerStates[(int)SamplerState::SS_PointClamp]);
	if (FAILED(result))
	{
		return false;
	}

	pointSampleDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	pointSampleDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	pointSampleDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	result = DX11::Device->CreateSamplerState(&pointSampleDesc, &mySamplerStates[(int)SamplerState::SS_PointWrap]);
	if(FAILED(result))
	{
		return false;
	}


	SetSamplerState(SamplerState::SS_Default, 0);
	SetSamplerState(SamplerState::SS_Wrap, 1);
	SetSamplerState(SamplerState::SS_PointClamp, 2);
	SetSamplerState(SamplerState::SS_PointClamp, 3);

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

void RendererBase::RemoveResource(int aSlot)
{
	ID3D11ShaderResourceView* nullsrv = nullptr;
	DX11::Context->PSSetShaderResources(aSlot, 1, &nullsrv);
}
