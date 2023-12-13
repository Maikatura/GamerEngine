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
	result = DX11::Get().GetDevice()->CreateBuffer(&bufferDescription, &vxSubresource, myInstanceBuffer.GetAddressOf());
	if(FAILED(result))
	{
		return false;
	}


	{
		D3D11_SAMPLER_DESC samplerStateDesc{};
		samplerStateDesc.Filter = D3D11_FILTER_ANISOTROPIC;
		samplerStateDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerStateDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerStateDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerStateDesc.MipLODBias = 0.0f;
		samplerStateDesc.MaxAnisotropy = D3D11_DEFAULT_MAX_ANISOTROPY;
		samplerStateDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		samplerStateDesc.BorderColor[0] = 1.0f;
		samplerStateDesc.BorderColor[1] = 1.0f;
		samplerStateDesc.BorderColor[2] = 1.0f;
		samplerStateDesc.BorderColor[3] = 1.0f;
		samplerStateDesc.MinLOD = -D3D11_FLOAT32_MAX;
		samplerStateDesc.MaxLOD = D3D11_FLOAT32_MAX;

		HRESULT result = DX11::Get().GetDevice()->CreateSamplerState(&samplerStateDesc, mySamplerStates[(int)SamplerState::Default].GetAddressOf());

		if(FAILED(result))
		{
			return false;
		}
	}

	{
		D3D11_SAMPLER_DESC samplerStateDesc{};
		samplerStateDesc.Filter = D3D11_FILTER_ANISOTROPIC;
		samplerStateDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerStateDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerStateDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerStateDesc.MipLODBias = 0.0f;
		samplerStateDesc.MaxAnisotropy = D3D11_DEFAULT_MAX_ANISOTROPY;
		samplerStateDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		samplerStateDesc.BorderColor[0] = 1.0f;
		samplerStateDesc.BorderColor[1] = 1.0f;
		samplerStateDesc.BorderColor[2] = 1.0f;
		samplerStateDesc.BorderColor[3] = 1.0f;
		samplerStateDesc.MinLOD = -D3D11_FLOAT32_MAX;
		samplerStateDesc.MaxLOD = D3D11_FLOAT32_MAX;

		HRESULT result = DX11::Get().GetDevice()->CreateSamplerState(&samplerStateDesc, mySamplerStates[(int)SamplerState::Wrap].GetAddressOf());

		if(FAILED(result))
		{
			return false;
		}
	}

	{
		D3D11_SAMPLER_DESC samplerStateDesc;
		samplerStateDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		samplerStateDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerStateDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerStateDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerStateDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		samplerStateDesc.MipLODBias = 0.f;
		samplerStateDesc.MaxAnisotropy = 1;
		samplerStateDesc.MinLOD = -D3D11_FLOAT32_MAX;
		samplerStateDesc.MaxLOD = D3D11_FLOAT32_MAX;

		HRESULT result = DX11::Get().GetDevice()->CreateSamplerState(&samplerStateDesc, mySamplerStates[(int)SamplerState::PointClamp].GetAddressOf());
		if(FAILED(result))
		{
			return false;
		}
	}

	{
		D3D11_SAMPLER_DESC samplerStateDesc;
		samplerStateDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		samplerStateDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerStateDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerStateDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerStateDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		samplerStateDesc.MipLODBias = 0.f;
		samplerStateDesc.MaxAnisotropy = 1;
		samplerStateDesc.MinLOD = -D3D11_FLOAT32_MAX;
		samplerStateDesc.MaxLOD = D3D11_FLOAT32_MAX;

		HRESULT result = DX11::Get().GetDevice()->CreateSamplerState(&samplerStateDesc, mySamplerStates[(int)SamplerState::PointWrap].GetAddressOf());
		if(FAILED(result))
		{
			return false;
		}
	}

	{
		D3D11_SAMPLER_DESC samplerStateDesc{};
		samplerStateDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerStateDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerStateDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerStateDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerStateDesc.MipLODBias = 0.0f;
		samplerStateDesc.MaxAnisotropy = 1;
		samplerStateDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		samplerStateDesc.BorderColor[0] = 1.0f;
		samplerStateDesc.BorderColor[1] = 1.0f;
		samplerStateDesc.BorderColor[2] = 1.0f;
		samplerStateDesc.BorderColor[3] = 1.0f;
		samplerStateDesc.MinLOD = -D3D11_FLOAT32_MAX;
		samplerStateDesc.MaxLOD = D3D11_FLOAT32_MAX;

		HRESULT result = DX11::Get().GetDevice()->CreateSamplerState(&samplerStateDesc, mySamplerStates[(int)SamplerState::LinearClamp].GetAddressOf());

		if(FAILED(result))
		{
			return false;
		}
	}

	{
		D3D11_SAMPLER_DESC samplerStateDesc{};
		samplerStateDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerStateDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerStateDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerStateDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerStateDesc.MipLODBias = 0.0f;
		samplerStateDesc.MaxAnisotropy = 1;
		samplerStateDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		samplerStateDesc.BorderColor[0] = 1.0f;
		samplerStateDesc.BorderColor[1] = 1.0f;
		samplerStateDesc.BorderColor[2] = 1.0f;
		samplerStateDesc.BorderColor[3] = 1.0f;
		samplerStateDesc.MinLOD = -D3D11_FLOAT32_MAX;
		samplerStateDesc.MaxLOD = D3D11_FLOAT32_MAX;

		HRESULT result = DX11::Get().GetDevice()->CreateSamplerState(&samplerStateDesc, mySamplerStates[(int)SamplerState::LinearWrap].GetAddressOf());

		if(FAILED(result))
		{

			return false;
		}
	}

	{
		D3D11_SAMPLER_DESC samplerStateDesc{};
		samplerStateDesc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
		samplerStateDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerStateDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerStateDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerStateDesc.MipLODBias = 0.0f;
		samplerStateDesc.MaxAnisotropy = 1;
		samplerStateDesc.ComparisonFunc = D3D11_COMPARISON_LESS;
		samplerStateDesc.BorderColor[0] = 1.0f;
		samplerStateDesc.BorderColor[1] = 1.0f;
		samplerStateDesc.BorderColor[2] = 1.0f;
		samplerStateDesc.BorderColor[3] = 1.0f;
		samplerStateDesc.MinLOD = -D3D11_FLOAT32_MAX;
		samplerStateDesc.MaxLOD = D3D11_FLOAT32_MAX;

		HRESULT result = DX11::Get().GetDevice()->CreateSamplerState(&samplerStateDesc, mySamplerStates[(int)SamplerState::ComparisonLinearClamp].GetAddressOf());

		if(FAILED(result))
		{
			return false;
		}
	}



	{

		D3D11_BLEND_DESC blendDesc{};
		blendDesc.RenderTarget[0].BlendEnable = TRUE;
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_MAX;
		blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		result = DX11::Get().GetDevice()->CreateBlendState(&blendDesc, &myBlendStates[(int)BlendState::Alpha]);
		if(FAILED(result))
		{
			return false;
		}
	}

	{
		D3D11_BLEND_DESC blendDesc{};
		blendDesc.RenderTarget[0].BlendEnable = TRUE;
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_MAX;
		blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		result = DX11::Get().GetDevice()->CreateBlendState(&blendDesc, &myBlendStates[(int)BlendState::Additive]);
		if(FAILED(result))
		{
			return false;
		}
	}


	{
		D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {};
		depthStencilDesc.DepthEnable = TRUE;
		depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

		HRESULT result = DX11::Get().GetDevice()->CreateDepthStencilState(&depthStencilDesc, &myDepthStencilStates[(int)DepthStencilState::ReadWrite]);

		if(FAILED(result))
		{
			
			return false;
		}
	}

	{
		D3D11_DEPTH_STENCIL_DESC depthStencilDesc{};
		depthStencilDesc.DepthEnable = TRUE;
		depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

		HRESULT result = DX11::Get().GetDevice()->CreateDepthStencilState(&depthStencilDesc, &myDepthStencilStates[(int)DepthStencilState::ReadOnly]);

		if(FAILED(result))
		{
			
			return false;
		}
	}

	{
		D3D11_DEPTH_STENCIL_DESC depthStencilDesc{};
		depthStencilDesc.DepthEnable = TRUE;
		depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

		HRESULT result = DX11::Get().GetDevice()->CreateDepthStencilState(&depthStencilDesc, &myDepthStencilStates[(int)DepthStencilState::ReadOnlyLEqual]);;

		if(FAILED(result))
		{
			
			return false;
		}
	}

	{
		D3D11_DEPTH_STENCIL_DESC depthStencilDesc{};
		depthStencilDesc.DepthEnable = TRUE;
		depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

		depthStencilDesc.StencilEnable = TRUE;
		depthStencilDesc.StencilReadMask = 0xFF;
		depthStencilDesc.StencilWriteMask = 0xFF;

		depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		HRESULT result = DX11::Get().GetDevice()->CreateDepthStencilState(&depthStencilDesc, &myDepthStencilStates[(int)DepthStencilState::StencilKeep]);;

		if(FAILED(result))
		{
			
			return false;
		}
	}

	{
		D3D11_DEPTH_STENCIL_DESC depthStencilDesc{};
		depthStencilDesc.DepthEnable = TRUE;
		depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

		depthStencilDesc.StencilEnable = TRUE;
		depthStencilDesc.StencilReadMask = 0xFF;
		depthStencilDesc.StencilWriteMask = 0xFF;

		depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_ZERO;
		depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_ZERO;
		depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_ZERO;
		depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_ZERO;
		depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		HRESULT result = DX11::Get().GetDevice()->CreateDepthStencilState(&depthStencilDesc, &myDepthStencilStates[(int)DepthStencilState::StencilZero]);;

		if(FAILED(result))
		{
			return false;
		}
	}

	{
		D3D11_DEPTH_STENCIL_DESC depthStencilDesc{};
		depthStencilDesc.DepthEnable = FALSE;

		HRESULT result = DX11::Get().GetDevice()->CreateDepthStencilState(&depthStencilDesc, &myDepthStencilStates[(int)DepthStencilState::Disabled]);;

		if(FAILED(result))
		{
			return false;
		}
	}

	return true;
}

void RendererBase::SetDepthStencilState(DepthStencilState aDepthStencilState)
{
	//DX11::Get().GetContext()->OMSetDepthStencilState(DX11::Get(, 1);
	DX11::Get().GetContext()->OMSetDepthStencilState(myDepthStencilStates[(int)aDepthStencilState].Get(), 0xffffffff);
}

void RendererBase::SetBlendState(BlendState aBlendState)
{
	DX11::Get().GetContext()->OMSetBlendState(myBlendStates[(int)aBlendState].Get(), nullptr, 0xffffffff);
}

void RendererBase::SetSamplerState(int aSlot, SamplerState aSamplerState) 
{
	DX11::Get().GetContext()->PSSetSamplers(aSlot, 1, mySamplerStates[(int)aSamplerState].GetAddressOf());
}

void RendererBase::RemoveResource(int aSlot)
{
	//ID3D11ShaderResourceView* nullsrv = nullptr;
	//DX11::Get().GetContext()->PSSetShaderResources(aSlot, 1, &nullsrv);
}
