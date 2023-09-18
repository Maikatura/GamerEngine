#include <GraphicsEngine.pch.h>
#include <Renderer/Framework/DX11.h>
#include <Renderer/Model/Vertex.h>
#include <vector>
#include <fstream>
#include <Renderer/Model/Model.h>

#include "openvr.h"

ComPtr<ID3D11Device> DX11::Device;
ComPtr<ID3D11DeviceContext> DX11::Context;
ComPtr<IDXGISwapChain> DX11::SwapChain;

ComPtr<ID3D11SamplerState> DX11::SampleStateDefault;
ComPtr<ID3D11SamplerState> DX11::SamplerStateWrap;

ComPtr<ID3D11RenderTargetView> DX11::BackBuffer;
ComPtr<ID3D11ShaderResourceView> DX11::BackBufferSRV;
ID3D11Texture2D* DX11::BackBufferTex;
ComPtr<ID3D11DepthStencilView> DX11::DepthBuffer;

ComPtr<ID3D11RenderTargetView> DX11::RenderRTV;
ComPtr<ID3D11ShaderResourceView> DX11::RenderSRV;

ComPtr<ID3D11RenderTargetView> DX11::IDBuffer;
ComPtr<ID3D11Texture2D> DX11::IDBufferTex;
ComPtr<ID3D11Texture2D> DX11::StagingTex;
D3D11_TEXTURE2D_DESC DX11::IDBufferDesc;
D3D11_TEXTURE2D_DESC DX11::StagingTexDesc;


vr::IVRSystem* DX11::m_pHMD;
vr::IVRRenderModels* DX11::m_pRenderModels;
vr::TrackedDevicePose_t DX11::m_rTrackedDevicePose[vr::k_unMaxTrackedDeviceCount];
Matrix4x4f DX11::m_rmat4DevicePose[vr::k_unMaxTrackedDeviceCount];
Matrix4x4f DX11::m_mat4HMDPose;

D3D_FEATURE_LEVEL		DX11::featureLevel;
D3D_DRIVER_TYPE			DX11::driverType;

ID3D11Texture2D* DX11::myBackBufferTex;

uint32_t DX11::m_nRenderWidth;
uint32_t DX11::m_nRenderHeight;

ID3D11RenderTargetView* DX11::myRenderTargetView = nullptr;
ID3D11DepthStencilView* DX11::myDepthStencilView = nullptr;

D3D11_VIEWPORT DX11::myViewport;
ID3D11DepthStencilState* DX11::pDSState;
ID3D11DepthStencilState* DX11::myDepthDisabledStencilState;
ID3D11DeviceContext* DX11::myImmediateContext = nullptr;

std::shared_ptr<RenderTexture> DX11::m_RenderTextureLeft;
std::shared_ptr<RenderTexture> DX11::m_RenderTextureRight;
std::shared_ptr<RenderTexture> DX11::myScreenView;

ID3D11RasterizerState* DX11::myFrontCulling;
ID3D11RasterizerState* DX11::myBackCulling;


DX11::DX11()
{}

DX11::~DX11()
{}

ID3D11Device* DX11::GetDevice()
{
	return Device.Get();
}

ID3D11DeviceContext* DX11::GetContext()
{
	return myImmediateContext;
}

ID3D11DepthStencilView* DX11::GetDepthStencilView()
{
	return myDepthStencilView;
}

void DX11::TurnZBufferOn()
{
	myImmediateContext->OMSetDepthStencilState(pDSState, 1);
	return;
}

void DX11::TurnZBufferOff()
{

	myImmediateContext->OMSetDepthStencilState(myDepthDisabledStencilState, 1);
	return;
}


bool DX11::Init(HWND aWindowHandle, bool aEnableDeviceDebug, bool aEnabledVR)
{
	WindowHandle = aWindowHandle;


	UINT createDeviceFlags = 0;

#if _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

#ifndef VR_DISABLED

	// Loading the SteamVR Runtime
	vr::EVRInitError eError = vr::VRInitError_None;

	m_pHMD = vr::VR_Init(&eError, vr::VRApplication_Scene);

	if (eError != vr::VRInitError_None)
	{
		m_pHMD = NULL;
		char buf[1024];
		sprintf_s(buf, ARRAYSIZE(buf), "Unable to init VR runtime: %s", vr::VR_GetVRInitErrorAsEnglishDescription(eError));
		std::string temp(buf);
		std::wstring wtemp(temp.begin(), temp.end());
		MessageBox(WindowHandle, wtemp.c_str(), L"VR_Init Failed", 0);
		return false;
	}

	m_pHMD->GetRecommendedRenderTargetSize(&m_nRenderWidth, &m_nRenderHeight);

	printf("width = %d, height = %d", m_nRenderWidth, m_nRenderHeight);

	//m_nRenderWidth /= 2;
	//m_nRenderHeight /= 4;

	//clientWidth = m_nRenderWidth;
	//clientHeight = m_nRenderHeight;

	m_pRenderModels = (vr::IVRRenderModels*)vr::VR_GetGenericInterface(vr::IVRRenderModels_Version, &eError);
	if (!m_pRenderModels)
	{
		m_pHMD = NULL;
		vr::VR_Shutdown();

		char buf[1024];
		sprintf_s(buf, ARRAYSIZE(buf), "Unable to get render model interface: %s", vr::VR_GetVRInitErrorAsEnglishDescription(eError));
		std::string temp(buf);
		std::wstring wtemp(temp.begin(), temp.end());
		MessageBox(WindowHandle, wtemp.c_str(), L"VR_Init Failed", NULL);
		return false;
	}

	if (!vr::VRCompositor())
	{
		printf("Compositor initialization failed. See log file for details\n");
		return false;
	}

#endif

	RECT clientRect = { 0,0,0,0 };
	GetClientRect(WindowHandle, &clientRect);

	if (m_nRenderWidth == 0)
	{
		m_nRenderWidth = clientRect.right - clientRect.left;
	}

	if (m_nRenderHeight == 0)
	{
		m_nRenderHeight = clientRect.bottom - clientRect.top;
	}

	// CREATE DEVICE AND SWAP CHAIN
	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE, // the first thing to try, if failed, go to the next
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE
	};
	UINT numDriverTypes = ARRAYSIZE(driverTypes);

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0, // texture size and others..
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3
	};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	HRESULT result = S_FALSE;

	DXGI_SWAP_CHAIN_DESC swapDesc;
	ZeroMemory(&swapDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
	swapDesc.BufferCount = 1;
	swapDesc.BufferDesc.Width = clientRect.right - clientRect.left;
	swapDesc.BufferDesc.Height = clientRect.bottom - clientRect.top;
	swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // unsigned normal
	swapDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapDesc.OutputWindow = WindowHandle;
	swapDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapDesc.Windowed = true;
	swapDesc.SampleDesc.Count = 1; // multisampling, which antialiasing for geometry. Turn it off
	swapDesc.SampleDesc.Quality = 0;
	swapDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; // alt-enter fullscreen

	swapDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	HRESULT errorCode;
	for (unsigned i = 0; i < numDriverTypes; ++i)
	{
		errorCode = D3D11CreateDeviceAndSwapChain(NULL, driverTypes[i], NULL, createDeviceFlags,
			featureLevels, numFeatureLevels, D3D11_SDK_VERSION, &swapDesc, &SwapChain, &Device,
			&featureLevel, &myImmediateContext);

		if (SUCCEEDED(errorCode))
		{
			driverType = driverTypes[i];
			break;
		}
	}

	if (FAILED(errorCode))
	{
		//OutputDebugString(_T("FAILED TO CREATE DEVICE AND SWAP CHAIN"));
		//MyDebug(_T("FAILED TO CREATE DEVICE AND SWAP CHAIN"));
		return false;
	}

	// CREATE RENDER TARGET VIEW
	result = SwapChain->GetBuffer(NULL, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&myBackBufferTex));
	if (FAILED(result))
	{
		return false;
	}

	D3D11_TEXTURE2D_DESC BBDesc;
	ZeroMemory(&BBDesc, sizeof(D3D11_TEXTURE2D_DESC));
	myBackBufferTex->GetDesc(&BBDesc);

	D3D11_RENDER_TARGET_VIEW_DESC RTVDesc;
	ZeroMemory(&RTVDesc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
	RTVDesc.Format = BBDesc.Format;
	RTVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	//RTVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMS;
	RTVDesc.Texture2D.MipSlice = 0;

	result = Device->CreateRenderTargetView(myBackBufferTex, &RTVDesc, &myRenderTargetView);
	if (FAILED(result))
	{
		//MyDebug(_T("ERROR"));
	}
	SafeRelease(myBackBufferTex);

	// CREATE DEPTH STENCIL
	ID3D11Texture2D* pDepthStencil = NULL;
	D3D11_TEXTURE2D_DESC descDepth;
	ZeroMemory(&descDepth, sizeof(descDepth));
	descDepth.Width = m_nRenderWidth;// swapDesc.BufferDesc.Width;
	descDepth.Height = m_nRenderHeight;// swapDesc.BufferDesc.Height;
	descDepth.MipLevels = 1;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;// DXGI_FORMAT_D32_FLOAT;//DXGI_FORMAT_D24_UNORM_S8_UINT;;//pDeviceSettings->d3d11.AutoDepthStencilFormat;
	// DXGI_FORMAT_D32_FLOAT_S8X24_UINT
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	descDepth.ArraySize = 1;


	result = Device->CreateTexture2D(&descDepth, NULL, &pDepthStencil);
	if (FAILED(result))
		return false;


	D3D11_DEPTH_STENCIL_DESC dsDesc;
	ZeroMemory(&dsDesc, sizeof(dsDesc));
	// Depth test parameters
	dsDesc.DepthEnable = true;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;

	// Stencil test parameters
	dsDesc.StencilEnable = true;
	dsDesc.StencilReadMask = 0xFF;
	dsDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing
	dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing
	dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create depth stencil state
	result = Device->CreateDepthStencilState(&dsDesc, &pDSState);
	if (FAILED(result))
	{
		return false;
	}

	// Bind depth stencil state
	myImmediateContext->OMSetDepthStencilState(pDSState, 1);



	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory(&descDSV, sizeof(descDSV));
	descDSV.Format = descDepth.Format;// DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	//descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
	descDSV.Texture2D.MipSlice = 0;

	// Create the depth stencil view
	result = Device->CreateDepthStencilView(pDepthStencil, // Depth stencil texture
		&descDSV, // Depth stencil desc
		&myDepthStencilView);  // [out] Depth stencil view

	if (FAILED(result))
	{
		WCHAR buf[100];
		wsprintf(buf, L"%x", result);
		/*MyDebug(buf);
		MyDebug(L"CreateDepthStencilView failed.");*/
		return false;
	}

	//BIND RENDER TARGET VIEW
	myImmediateContext->OMSetRenderTargets(1, &myRenderTargetView, myDepthStencilView); // depth stencil view is for shadow map


	D3D11_DEPTH_STENCIL_DESC depthDisabledStencilDesc;
	// Clear the second depth stencil state before setting the parameters.
	ZeroMemory(&depthDisabledStencilDesc, sizeof(depthDisabledStencilDesc));

	// Now create a second depth stencil state which turns off the Z buffer for 2D rendering.  The only difference is 
	// that DepthEnable is set to false, all other parameters are the same as the other depth stencil state.
	depthDisabledStencilDesc.DepthEnable = false;
	depthDisabledStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthDisabledStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthDisabledStencilDesc.StencilEnable = true;
	depthDisabledStencilDesc.StencilReadMask = 0xFF;
	depthDisabledStencilDesc.StencilWriteMask = 0xFF;
	depthDisabledStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthDisabledStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depthDisabledStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthDisabledStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;


	

	// Create the state using the device.
	result = Device->CreateDepthStencilState(&depthDisabledStencilDesc, &myDepthDisabledStencilState);
	if (FAILED(result))
	{
		return false;
	}


	//D3D11_RASTERIZER_DESC rasterizerDesc = {};
	//rasterizerDesc.FillMode = D3D11_FILL_SOLID; // or D3D11_FILL_WIREFRAME for wireframe rendering
	//rasterizerDesc.CullMode = D3D11_CULL_BACK;  // or D3D11_CULL_NONE to disable backface culling
	//rasterizerDesc.FrontCounterClockwise = FALSE; // Set to TRUE if your models use counterclockwise winding order for front faces
	//rasterizerDesc.DepthClipEnable = TRUE; // Enable depth clipping

	//// Create the rasterizer state
	//ID3D11RasterizerState* rasterizerState;
	//Device->CreateRasterizerState(&rasterizerDesc, &rasterizerState);


	D3D11_RASTERIZER_DESC rasterizerDesc = {};
	rasterizerDesc.FillMode = D3D11_FILL_SOLID; // or D3D11_FILL_WIREFRAME for wireframe rendering
	rasterizerDesc.CullMode = D3D11_CULL_NONE;  // or D3D11_CULL_NONE to disable backface culling
	rasterizerDesc.FrontCounterClockwise = true; // Set to TRUE if your models use counterclockwise winding order for front faces
	rasterizerDesc.DepthClipEnable = TRUE; // Enable depth clipping

	
	Device->CreateRasterizerState(&rasterizerDesc, &myFrontCulling);

	// Set the rasterizer state
	//myImmediateContext->RSSetState(myFrontCulling);

	
	rasterizerDesc.FillMode = D3D11_FILL_SOLID; // or D3D11_FILL_WIREFRAME for wireframe rendering
	rasterizerDesc.CullMode = D3D11_CULL_BACK;  // or D3D11_CULL_NONE to disable backface culling
	rasterizerDesc.FrontCounterClockwise = FALSE; // Set to TRUE if your models use counterclockwise winding order for front faces
	rasterizerDesc.DepthClipEnable = TRUE; // Enable depth clipping

	
	Device->CreateRasterizerState(&rasterizerDesc, &myBackCulling);

	// Set the rasterizer state
	//myImmediateContext->RSSetState(rasterizerState);



	//VIEWPORT CREATION
	myViewport.Width = static_cast<float>(m_nRenderWidth);
	myViewport.Height = static_cast<float>(m_nRenderHeight);
	myViewport.TopLeftX = 0;
	myViewport.TopLeftY = 0;
	myViewport.MinDepth = 0.0f;
	myViewport.MaxDepth = 1.0f;

	// BIND VIEWPORT
	myImmediateContext->RSSetViewports(1, &myViewport);

	// Create the render to texture object.
	m_RenderTextureLeft = std::make_shared<RenderTexture>();
	if (!m_RenderTextureLeft)
	{
		return false;
	}

	// Initialize the render to texture object.
	result = m_RenderTextureLeft->Initialize(Device.Get(), m_nRenderWidth, m_nRenderHeight);
	if (!result)
	{
		return false;
	}

	m_RenderTextureRight = std::make_shared<RenderTexture>();
	if (!m_RenderTextureRight)
	{
		return false;
	}

	// Initialize the render to texture object.
	result = m_RenderTextureRight->Initialize(Device.Get(), m_nRenderWidth, m_nRenderHeight);
	if (!result)
	{
		return false;
	}

	// Create the render to texture object.
	myScreenView = std::make_shared<RenderTexture>();
	if (!myScreenView)
	{
		return false;
	}

	//m_nRenderHeight = clientRect.bottom - clientRect.top;
	

	// Initialize the render to texture object.
	result = myScreenView->Initialize(Device.Get(), m_nRenderWidth, m_nRenderHeight);
	if (!result)
	{
		return false;
	}

	//CreateSampler();

	//result = CreateSwapChain(aEnableDeviceDebug);
	//if(!result)
	//{
	//	return false;
	//}

	//result = CreateTexture2D();
	//if(!result)
	//{
	//	return false;
	//}

	//result = CreateDepthBuffer();
	//if(!result)
	//{
	//	return false;
	//}

	//result = ResizeViewport();
	//if(!result)
	//{
	//	return false;
	//}

	//result = CreateShaderResourceView();
	//if(!result)
	//{
	//	return false;
	//}

	//result = CreateSampler();
	//if(!result)
	//{
	//	return false;
	//}

	//result = CreateSelectionView();
	//if(!result)
	//{
	//	return false;
	//}

	//if (aEnabledVR)
	//{

	//	// Initialize the render to texture object.
	//	m_RenderTextureLeft = RenderTexture::Create(m_nRenderWidth, m_nRenderHeight, DXGI_FORMAT_R8G8B8A8_UNORM);
	//	if (!result)
	//	{
	//		return false;
	//	}

	//	

	//	// Initialize the render to texture object.
	//	m_RenderTextureRight = RenderTexture::Create(m_nRenderWidth, m_nRenderHeight, DXGI_FORMAT_R8G8B8A8_UNORM);
	//	if (!result)
	//	{
	//		return false;
	//	}
	//}

#ifndef VR_DISABLED
	if (!vr::VRCompositor())
	{
		printf("Compositor initialization failed. See log file for details\n");
		return false;
	}
#endif

	return true;
}

void DX11::BeginFrame(std::array<float, 4> aClearColor)
{
	/*GetContext()->ClearRenderTargetView(BackBuffer.Get(), &aClearColor[0]);
	GetContext()->ClearRenderTargetView(RenderRTV.Get(), &aClearColor[0]);
	GetContext()->ClearDepthStencilView(DepthBuffer.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	GetContext()->ClearRenderTargetView(IDBuffer.Get(), &aClearColor[0]);*/
}

void DX11::EndFrame()
{
	TurnZBufferOn();

	SwapChain->Present(0, 0);

#ifndef VR_DISABLED

	vr::Texture_t leftEyeTexture = { m_RenderTextureLeft->GetTexture(), vr::TextureType_DirectX, vr::ColorSpace_Auto };
	vr::EVRCompositorError error1 = vr::VRCompositor()->Submit(vr::Eye_Left, &leftEyeTexture);
	vr::Texture_t rightEyeTexture = { m_RenderTextureRight->GetTexture(), vr::TextureType_DirectX, vr::ColorSpace_Auto };
	vr::VRCompositor()->Submit(vr::Eye_Right, &rightEyeTexture);
	if (error1)
		printf("error is %d \n", error1);

#endif

}

RECT DX11::GetClientSize()
{
	RECT clientRect;
	GetClientRect(WindowHandle, &clientRect);
	return clientRect;
}

UINT DX11::GetScreenObjectId(UINT x, UINT y)
{
	if (x >= IDBufferDesc.Width) return 0;
	if (y >= IDBufferDesc.Height) return 0;

	D3D11_BOX b = { x, y, 0, x + 1, y + 1, 1 };
	GetContext()->CopySubresourceRegion(StagingTex.Get(), 0, 0, 0, 0, IDBufferTex.Get(), 0, &b);

	D3D11_MAPPED_SUBRESOURCE mapped;
	HRESULT h = GetContext()->Map(StagingTex.Get(), 0, D3D11_MAP_READ, 0, &mapped);
	UINT32* p = (UINT32*)mapped.pData;
	GetContext()->Unmap(StagingTex.Get(), 0);
	//const FLOAT clear[4] = { 0, 0, 0, 0 };
	//DX11::ourContext->ClearRenderTargetView(DX11::ourIDBuffer.Get(), clear);
	if (p == NULL) return 0;
	return *p;
}

bool DX11::CreateSwapChain(bool aEnableDeviceDebug)
{
	RECT clientRect = { 0,0,0,0 };
	GetClientRect(WindowHandle, &clientRect);

	HRESULT result = S_FALSE;

	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
	ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
	swapChainDesc.BufferDesc.Width = clientRect.right - clientRect.left;
	swapChainDesc.BufferDesc.Height = clientRect.bottom - clientRect.top;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 1;
	swapChainDesc.OutputWindow = WindowHandle;
	swapChainDesc.Windowed = TRUE;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = 0;

	constexpr UINT numberOfFeatureLevels = 1;
	D3D_FEATURE_LEVEL featureLevels[numberOfFeatureLevels] =
	{
		//D3D_FEATURE_LEVEL_11_1, --add this if supported by the device, otherwise everything will crash!
		D3D_FEATURE_LEVEL_11_0
	};

	result = D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		aEnableDeviceDebug ? D3D11_CREATE_DEVICE_DEBUG : 0,
		featureLevels,
		numberOfFeatureLevels,
		D3D11_SDK_VERSION,
		&swapChainDesc,
		SwapChain.GetAddressOf(),
		Device.GetAddressOf(),
		nullptr,
		Context.GetAddressOf()
	);
	if (FAILED(result))
		return false;


	return true;
}

bool DX11::CreateTexture2D()
{

	RECT clientRect = { 0,0,0,0 };
	GetClientRect(WindowHandle, &clientRect);

	HRESULT result = S_FALSE;


	result = SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&BackBufferTex);
	if (FAILED(result))
		return false;


	result = Device->CreateRenderTargetView(BackBufferTex, nullptr, BackBuffer.GetAddressOf());
	if (FAILED(result))
		return false;

	SafeRelease(BackBufferTex);

	return true;
}

bool DX11::CreateDepthBuffer()
{
	HRESULT result = S_FALSE;
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
	if (FAILED(result))
		return false;

	result = Device->CreateDepthStencilView(depthBufferTexture.Get(), nullptr, DepthBuffer.GetAddressOf());
	if (FAILED(result))
		return false;

	return true;
}

bool DX11::ResizeViewport()
{
	/*RECT clientRect = { 0,0,0,0 };
	GetClientRect(WindowHandle, &clientRect);


	D3D11_VIEWPORT viewport = {};
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
	viewport.Width = static_cast<FLOAT>(clientRect.right - clientRect.left);
	viewport.Height = static_cast<FLOAT>(clientRect.bottom - clientRect.top);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	GetContext()->RSSetViewports(1, &viewport);*/

	return true;
}

bool DX11::CreateShaderResourceView()
{
	HRESULT result = S_FALSE;


	RECT clientRect = { 0,0,0,0 };
	GetClientRect(WindowHandle, &clientRect);

	D3D11_TEXTURE2D_DESC textureDesc{};
	ZeroMemory(&textureDesc, sizeof(textureDesc));

	textureDesc.Width = static_cast<UINT>(clientRect.right - clientRect.left);
	textureDesc.Height = static_cast<UINT>(clientRect.bottom - clientRect.top);
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;

	result = Device->CreateTexture2D(&textureDesc, nullptr, &BackBufferTex);
	if (FAILED(result))
		return false;

	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc{};
	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc{};
	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;


	result = Device->CreateRenderTargetView(BackBufferTex, &renderTargetViewDesc, RenderRTV.GetAddressOf());
	if (FAILED(result))
		return false;

	result = Device->CreateShaderResourceView(BackBufferTex, &shaderResourceViewDesc, RenderSRV.GetAddressOf());
	if (FAILED(result))
		return false;

	SafeRelease(BackBufferTex);

	return true;
}

bool DX11::CreateSampler()
{
	HRESULT result = S_FALSE;
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
	if (FAILED(result))
		return false;

	GetContext()->PSSetSamplers(0, 1, SampleStateDefault.GetAddressOf());

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
	if (FAILED(result))
		return false;

	D3D11_RENDER_TARGET_VIEW_DESC IDBufferViewDesc{};
	IDBufferViewDesc.Format = IDBufferDesc.Format;
	IDBufferViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	IDBufferViewDesc.Texture2D.MipSlice = 0;

	result = Device->CreateRenderTargetView(IDBufferTex.Get(), &IDBufferViewDesc, IDBuffer.GetAddressOf());
	if (FAILED(result)) return false;

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
	if (FAILED(result))
		return false;

	return true;
}

void DX11::Resize()
{
	//	GetContext()->ClearState();
	//	ID3D11RenderTargetView* nullViews[] = { nullptr };
	//	GetContext()->OMSetRenderTargets(ARRAYSIZE(nullViews), nullViews, nullptr);
	//	BackBuffer->Release();
	//	DepthBuffer->Release();
	//	SampleStateDefault->Release();
	//	//SamplerStateWrap->Release();
	//	//BackBufferTex[0] = nullptr;
	//
	//
	//	if(BackBufferTex != nullptr)
	//	{
	//		BackBufferTex->Release();
	//	}
	//
	//	RenderRTV->Release();
	//	RenderSRV->Release();
	//	IDBuffer->Release();
	//	IDBufferTex->Release();
	//	StagingTex->Release();
	//
	//
	//	RECT clientRect = { 0,0,0,0 };
	//	GetClientRect(WindowHandle, &clientRect);
	//
	//	GetContext()->Flush();
	//
	//
	//#if _DEBUG
	//	ReportDX11();
	//#endif
	//
	//	HRESULT result;
	//	result = SwapChain->ResizeBuffers(1, static_cast<UINT>(clientRect.right - clientRect.left), static_cast<UINT>(clientRect.bottom - clientRect.top), DXGI_FORMAT_UNKNOWN, 0);
	//	if(result != S_OK)
	//	{
	//		std::cout << "SwapChain Could not resize." << "\n";
	//	}
	//
	//	ComPtr<ID3D11Texture2D> spBackBuffer;
	//	result = SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(spBackBuffer.GetAddressOf()));
	//	if(result != S_OK)
	//	{
	//		std::cout << "SwapChain could not get backbuffer 0." << "\n";
	//	}
	//
	//	CreateTexture2D();
	//	CreateDepthBuffer();
	//	ResizeViewport();
	//	CreateShaderResourceView();
	//	CreateSampler();
	//	CreateSelectionView();
}
