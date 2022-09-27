#include <GraphicsEngine.pch.h>
#include <Framework/DX11.h>
#include <Model/Vertex.h>
#include <vector>
#include <fstream>
#include <Model/Model.h>

ComPtr<ID3D11Device> DX11::Device;
ComPtr<ID3D11DeviceContext> DX11::Context;
ComPtr<IDXGISwapChain> DX11::SwapChain;

ComPtr<ID3D11SamplerState> DX11::SampleStateDefault;
ComPtr<ID3D11SamplerState> DX11::SamplerStateWrap;

ComPtr<ID3D11RenderTargetView> DX11::BackBuffer;
ComPtr<ID3D11DepthStencilView> DX11::DepthBuffer;

ComPtr<ID3D11RenderTargetView> DX11::RenderRTV;
ComPtr<ID3D11ShaderResourceView> DX11::RenderSRV;

ComPtr<ID3D11Texture2D> DX11::Texture2D;

DX11::DX11()
{
}

DX11::~DX11()
{
}

bool DX11::Init(HWND aWindowHandle, bool aEnableDeviceDebug)
{
	bool result = true;

	WindowHandle = aWindowHandle;

	result = CreateSwapChain(aEnableDeviceDebug);
	if (!result)
	{
		return false;
	}

	result = CreateTexture2D();
	if(!result)
	{
		return false;
	}

	result = CreateDepthBuffer();
	if(!result)
	{
		return false;
	}

	result = CreateViewport();
	if(!result)
	{
		return false;
	}

	result = CreateShaderResourceView();
	if(!result)
	{
		return false;
	}

	result = CreateSampler();
	if(!result)
	{
		return false;
	}

	return true;
}

void DX11::BeginFrame(std::array<float, 4> aClearColor)
{
	if(BackBuffer != NULL)
	{
		Context->ClearRenderTargetView(BackBuffer.Get(), &aClearColor[0]);
		Context->ClearRenderTargetView(RenderRTV.Get(), &aClearColor[0]);
		Context->ClearDepthStencilView(DepthBuffer.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}
}

void DX11::EndFrame()
{
	SwapChain->Present(1, 0);
}

bool DX11::CreateSwapChain(bool aEnableDeviceDebug)
{
	HRESULT result;

	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = WindowHandle;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.Windowed = true;


	result = D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		aEnableDeviceDebug ? D3D11_CREATE_DEVICE_DEBUG : 0,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&swapChainDesc,
		SwapChain.GetAddressOf(),
		Device.GetAddressOf(),
		nullptr,
		Context.GetAddressOf()
	);
	if(FAILED(result))
	{
		return false;
	}

	return true;
}

bool DX11::CreateTexture2D()
{

	RECT clientRect = { 0,0,0,0 };
	GetClientRect(WindowHandle, &clientRect);

	HRESULT result;

	ComPtr<ID3D11Texture2D> backBuffTex;
	result = SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), &backBuffTex);
	if(FAILED(result))
	{
		return false;
	}



	result = Device->CreateRenderTargetView(backBuffTex.Get(), nullptr, BackBuffer.GetAddressOf());
	if(FAILED(result))
	{
		return false;
	}

	return true;
}

bool DX11::CreateDepthBuffer()
{
	HRESULT result;
	RECT clientRect = { 0,0,0,0 };
	GetClientRect(WindowHandle, &clientRect);

	ComPtr<ID3D11Texture2D> depthBufferTexture;
	D3D11_TEXTURE2D_DESC depthBufferDesc = { 0 };
	depthBufferDesc.Width = clientRect.right - clientRect.left;
	depthBufferDesc.Height = clientRect.bottom - clientRect.top;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	result = Device->CreateTexture2D(&depthBufferDesc, nullptr, depthBufferTexture.GetAddressOf());
	if(FAILED(result))
	{
		return false;
	}

	result = Device->CreateDepthStencilView(depthBufferTexture.Get(), nullptr, DepthBuffer.GetAddressOf());
	if(FAILED(result))
	{
		return false;
	}

	return true;
}

bool DX11::CreateViewport()
{
	RECT clientRect = { 0,0,0,0 };
	GetClientRect(WindowHandle, &clientRect);

	Context->OMSetRenderTargets(1, BackBuffer.GetAddressOf(), DepthBuffer.Get());

	D3D11_VIEWPORT viewport = {};
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
	viewport.Width = static_cast<FLOAT>(clientRect.right - clientRect.left);
	viewport.Height = static_cast<FLOAT>(clientRect.bottom - clientRect.top);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	Context->RSSetViewports(1, &viewport);

	return true;
}

bool DX11::CreateShaderResourceView()
{
	D3D11_TEXTURE2D_DESC textureDesc;
	ZeroMemory(&textureDesc, sizeof(textureDesc));

	textureDesc.Width = 1280;
	textureDesc.Height = 720;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	Device->CreateTexture2D(&textureDesc, NULL, Texture2D.GetAddressOf());

	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	renderTargetViewDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	Device->CreateRenderTargetView(Texture2D.Get(), &renderTargetViewDesc, RenderRTV.GetAddressOf());


	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	shaderResourceViewDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;


	Device->CreateShaderResourceView(Texture2D.Get(), &shaderResourceViewDesc, RenderSRV.GetAddressOf());

	
	return true;
}

bool DX11::CreateSampler()
{
	HRESULT result;
	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.BorderColor[0] = 1.0f;
	samplerDesc.BorderColor[1] = 1.0f;
	samplerDesc.BorderColor[2] = 1.0f;
	samplerDesc.BorderColor[3] = 1.0f;
	samplerDesc.MinLOD = -D3D11_FLOAT32_MAX;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	result = Device->CreateSamplerState(&samplerDesc, SampleStateDefault.GetAddressOf());
	if(FAILED(result))
	{
		return false;
	}

	Context->PSSetSamplers(0, 1, SampleStateDefault.GetAddressOf());

	return true;
}

void DX11::Resize()
{
	ID3D11RenderTargetView* nullViews[] = { nullptr };
	Context->OMSetRenderTargets(0,0,0);
	BackBuffer = nullptr;
	DepthBuffer = nullptr;
	SampleStateDefault = nullptr;
	RenderRTV = nullptr;
	RenderSRV = nullptr;
	ComPtr<ID3D11Texture2D> backBuffTex;
	SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), &backBuffTex);
	backBuffTex = nullptr;

	RECT clientRect = { 0,0,0,0 };
	GetClientRect(WindowHandle, &clientRect);
	SwapChain->ResizeBuffers(1, static_cast<FLOAT>(clientRect.right - clientRect.left), static_cast<FLOAT>(clientRect.bottom - clientRect.top), DXGI_FORMAT_UNKNOWN, 0);

	CreateTexture2D();
	CreateDepthBuffer();
	CreateViewport();
	CreateShaderResourceView();
	CreateSampler();
}
