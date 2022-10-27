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
ComPtr<ID3D11Texture2D> DX11::BackBufferTex;
ComPtr<ID3D11DepthStencilView> DX11::DepthBuffer;

ComPtr<ID3D11RenderTargetView> DX11::RenderRTV;
ComPtr<ID3D11ShaderResourceView> DX11::RenderSRV;

ComPtr<ID3D11Texture2D> DX11::Texture2D[7];

ComPtr<ID3D11RenderTargetView> DX11::IDBuffer;
ComPtr<ID3D11Texture2D> DX11::IDBufferTex;
ComPtr<ID3D11Texture2D> DX11::StagingTex;
D3D11_TEXTURE2D_DESC DX11::IDBufferDesc;
D3D11_TEXTURE2D_DESC DX11::StagingTexDesc;

DX11::DX11()
{}

DX11::~DX11()
{}

bool DX11::Init(HWND aWindowHandle, bool aEnableDeviceDebug)
{
	bool result = true;

	WindowHandle = aWindowHandle;

	result = CreateSwapChain(aEnableDeviceDebug);
	if(!result)
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

	result = CreateSelectionView();
	if(!result)
	{
		return false;
	}

	return true;
}

void DX11::BeginFrame(std::array<float, 4> aClearColor)
{
	Context->ClearRenderTargetView(BackBuffer.Get(), &aClearColor[0]);
	Context->ClearRenderTargetView(RenderRTV.Get(), &aClearColor[0]);
	Context->ClearDepthStencilView(DepthBuffer.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	std::array<float, 4> test = { 0,0,0,0 };
	Context->ClearRenderTargetView(IDBuffer.Get(), test.data());



}

void DX11::EndFrame()
{
	SwapChain->Present(0, 0);
}

RECT DX11::GetClientSize()
{
	RECT clientRect;
	GetClientRect(WindowHandle, &clientRect);
	return clientRect;
}

UINT DX11::GetScreenObjectId(UINT x, UINT y)
{
	if(x >= IDBufferDesc.Width) return 0;
	if(y >= IDBufferDesc.Height) return 0;

	D3D11_BOX b = { x, y, 0, x + 1, y + 1, 1 };
	Context->CopySubresourceRegion(StagingTex.Get(), 0, 0, 0, 0, IDBufferTex.Get(), 0, &b);

	D3D11_MAPPED_SUBRESOURCE mapped;
	HRESULT h = Context->Map(StagingTex.Get(), 0, D3D11_MAP_READ, 0, &mapped);
	UINT32* p = (UINT32*)mapped.pData;
	Context->Unmap(StagingTex.Get(), 0);
	//const FLOAT clear[4] = { 0, 0, 0, 0 };
	//DX11::ourContext->ClearRenderTargetView(DX11::ourIDBuffer.Get(), clear);
	if(p == NULL) return 0;
	return *p;
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
	result = SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), &BackBufferTex);
	if(FAILED(result))
	{
		return false;
	}



	result = Device->CreateRenderTargetView(BackBufferTex.Get(), nullptr, BackBuffer.GetAddressOf());
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

	Context->OMSetRenderTargets(1, BackBuffer.GetAddressOf(), DepthBuffer.Get());

	return true;
}

bool DX11::CreateViewport()
{
	RECT clientRect = { 0,0,0,0 };
	GetClientRect(WindowHandle, &clientRect);


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
	RECT clientRect = { 0,0,0,0 };
	GetClientRect(WindowHandle, &clientRect);

	D3D11_TEXTURE2D_DESC textureDesc{};
	ZeroMemory(&textureDesc, sizeof(textureDesc));

	textureDesc.Width = static_cast<UINT>(clientRect.right - clientRect.left);
	textureDesc.Height = static_cast<UINT>(clientRect.bottom - clientRect.top);
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;

	for(size_t i = 0; i < 7; i++)
	{
		Device->CreateTexture2D(&textureDesc, nullptr, Texture2D[i].GetAddressOf());
	}


	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc{};
	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc{};
	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;


	Device->CreateRenderTargetView(Texture2D[0].Get(), &renderTargetViewDesc, RenderRTV.GetAddressOf());
	Device->CreateShaderResourceView(Texture2D[0].Get(), &shaderResourceViewDesc, RenderSRV.GetAddressOf());


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

bool DX11::CreateSelectionView()
{
	HRESULT result = S_OK;

	RECT clientRect = { 0,0,0,0 };
	GetClientRect(WindowHandle, &clientRect);

	D3D11_TEXTURE2D_DESC textureDesc;
	ZeroMemory(&textureDesc, sizeof(textureDesc));

	textureDesc.Width = static_cast<UINT>(clientRect.right - clientRect.left);
	textureDesc.Height = static_cast<UINT>(clientRect.bottom - clientRect.top);
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;

	IDBufferDesc = textureDesc;

	Device->CreateTexture2D(&textureDesc, nullptr, IDBufferTex.GetAddressOf());
	if(FAILED(result))
		return false;
	D3D11_RENDER_TARGET_VIEW_DESC IDBufferViewDesc;
	IDBufferViewDesc.Format = IDBufferDesc.Format;
	IDBufferViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	IDBufferViewDesc.Texture2D.MipSlice = 0;

	result = Device->CreateRenderTargetView(IDBufferTex.Get(), &IDBufferViewDesc, IDBuffer.GetAddressOf());
	if(FAILED(result)) return false;

	StagingTexDesc.Width = 1;
	StagingTexDesc.Height = 1;
	StagingTexDesc.ArraySize = 1;
	StagingTexDesc.Format = DXGI_FORMAT_R32_FLOAT;
	StagingTexDesc.SampleDesc.Count = 1;
	StagingTexDesc.Usage = D3D11_USAGE_STAGING;
	StagingTexDesc.BindFlags = 0;
	StagingTexDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	StagingTexDesc.MiscFlags = 0;
	result = Device->CreateTexture2D(&StagingTexDesc, NULL, StagingTex.GetAddressOf());
	if(FAILED(result))
		return false;

	return true;
}

void DX11::Resize()
{
	ID3D11RenderTargetView* nullViews[] = { nullptr };
	Context->OMSetRenderTargets(0, 0, 0);
	BackBuffer->Release();
	DepthBuffer->Release();
	SampleStateDefault->Release();

	for(int i = 0; i < 7; i++)
	{
		if(Texture2D[i] != nullptr)
		{
			Texture2D[i]->Release();
		}
	}
	RenderRTV->Release();
	RenderSRV->Release();
	IDBuffer->Release();
	IDBufferTex->Release();
	StagingTex->Release();

	ComPtr<ID3D11Texture2D> backBuffTex;
	SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), &backBuffTex);
	backBuffTex = nullptr;

	RECT clientRect = { 0,0,0,0 };
	GetClientRect(WindowHandle, &clientRect);
	SwapChain->ResizeBuffers(0, static_cast<UINT>(clientRect.right - clientRect.left), static_cast<UINT>(clientRect.bottom - clientRect.top), DXGI_FORMAT_UNKNOWN, 0);

	CreateTexture2D();
	CreateDepthBuffer();
	CreateViewport();
	CreateShaderResourceView();
	CreateSampler();
	CreateSelectionView();
}
