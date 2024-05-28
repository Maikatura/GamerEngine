#include <GraphicsEngine.pch.h>
#include "DX11.h"
#include "AssetHandlers/TextureAssetHandler.h"
#include "Utilites/VisualProfiler.h"


DX11::DX11(): BackBufferTex(nullptr), IDBufferDesc(), StagingTexDesc(), featureLevel(), driverType(),
              myBackBufferTex(nullptr),
              myRenderTargetView(nullptr),
              myDepthStencilSRV(nullptr),
              myViewport(),
              pDSState(nullptr),
              myDepthDisabledStencilState(nullptr),
              myImmediateContext(nullptr),
              myVRSystem(),
              myRenderWidth(0),
              myRenderHeight(0), myFrontCulling(nullptr),
              myBackCulling(nullptr)
{
}

DX11::~DX11()
= default;

ComPtr<IDXGISwapChain> DX11::GetSwapChain()
{
	return SwapChain;
}

ID3D11RasterizerState* DX11::GetFrontCulling() const
{
	return myFrontCulling;
}

void DX11::ClearRenderTargets(const Vector4f& aColor)
{
	myScreenView->ClearRenderTarget(myImmediateContext, nullptr, aColor);
	myImmediateContext->ClearRenderTargetView(myRenderTargetView, &aColor.x);

	constexpr float clearDepth = 1.0f;
	constexpr UINT8 clearStencil = 0;
	myImmediateContext->ClearDepthStencilView(myDepthStencilView->myDSV.Get(), D3D11_CLEAR_DEPTH, clearDepth, clearStencil);
}

DX11& DX11::Get()
{
	static DX11 instance;
	return instance;
}

void DX11::ResetRenderTarget(bool isUsingEditor, bool useDepth)
{
	if (isUsingEditor)
	{
		myScreenView->SetRenderTarget(myImmediateContext, (useDepth == true) ? myDepthStencilView->myDSV.Get() : nullptr);
	}
	else
	{
		myImmediateContext->OMSetRenderTargets(1, &myRenderTargetView, (useDepth == true) ? myDepthStencilView->myDSV.Get() : nullptr);
	}
}

Vector2ui DX11::GetScreenSize() const
{
	return Vector2ui{ myRenderWidth,myRenderHeight };
}

ID3D11Device* DX11::GetDevice() const
{
	return Device.Get();
}

ID3D11DeviceContext* DX11::GetContext() const
{
	return myImmediateContext;
}

Ref<DepthStencil> DX11::GetDepthStencilView()
{
	return myDepthStencilView;
}

ID3D11ShaderResourceView* DX11::GetDepthSRV() const
{
	return myDepthStencilSRV;
}

VRSystem& DX11::GetVRSystem()
{
	return myVRSystem;
}

void DX11::TurnZBufferOn()
{
	//myImmediateContext->OMSetDepthStencilState(pDSState, 1);
	return;
}

void DX11::TurnZBufferOff()
{

	//myImmediateContext->OMSetDepthStencilState(myDepthDisabledStencilState, 1);
	return;
}

Ref<RenderTexture>& DX11::GetLeftEyeView()
{
	return m_RenderTextureLeft;
}

Ref<RenderTexture>& DX11::GetRightEyeView()
{
	return m_RenderTextureRight;
}

Ref<RenderTexture>& DX11::GetScreenView()
{
	return myScreenView;
}

ID3D11RenderTargetView* DX11::GetRenderTargetView() const
{
	return myRenderTargetView;
}

bool DX11::IsVrNull()
{
	return myVRSystem.IsVrNull();
}


bool DX11::CreateRenderTargetView()
{
	// CREATE RENDER TARGET VIEW
	HRESULT result = SwapChain->GetBuffer(NULL, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&myBackBufferTex));
	GE_ASSERT(SUCCEEDED(result), "Failed to get swapchain buffer")
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
	SafeRelease(myBackBufferTex);
	GE_ASSERT(SUCCEEDED(result), "Failed to create Render Target view from back buffer")
		if (FAILED(result))
		{
			return false;
		}

	return true;
}

bool DX11::Init(HWND aWindowHandle, bool aEnableDeviceDebug, bool aEnabledVR)
{
	PROFILE_SCOPE("DX11::Init");
	WindowHandle = aWindowHandle;



#if ENABLE_VR
	myVRSystem.Init(WindowHandle);
#endif
	RECT clientRect = { 0,0,0,0 };
	GetClientRect(WindowHandle, &clientRect);

	if (myVRSystem.GetWidth() == 0)
	{
		//myVRSystem.SetWidth(clientRect.right - clientRect.left);
		myRenderWidth = clientRect.right - clientRect.left;
	}
	else
	{
		myRenderWidth = myVRSystem.GetWidth();
	}

	if (myVRSystem.GetHeight() == 0)
	{
		//myVRSystem.SetHeight(clientRect.bottom - clientRect.top);
		myRenderHeight = clientRect.bottom - clientRect.top;
	}
	else
	{
		myRenderHeight = myVRSystem.GetHeight();
	}

	if (!CreateSwapChain(aEnableDeviceDebug))
	{
		return false;
	}

	if (!CreateRenderTargetView())
	{
		return false;
	}

	

	//D3D11_DEPTH_STENCIL_DESC dsDesc;
	//ZeroMemory(&dsDesc, sizeof(dsDesc));
	//// Depth test parameters
	//dsDesc.DepthEnable = true;
	//dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	//dsDesc.DepthFunc = D3D11_COMPARISON_LESS;

	//// Stencil test parameters
	//dsDesc.StencilEnable = true;
	//dsDesc.StencilReadMask = 0xFF;
	//dsDesc.StencilWriteMask = 0xFF;

	//// Stencil operations if pixel is front-facing
	//dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	//dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	//dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	//dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	//// Stencil operations if pixel is back-facing
	//dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	//dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	//dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	//dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	//// Create depth stencil state
	//result = Device->CreateDepthStencilState(&dsDesc, &pDSState);
	//GE_ASSERT(SUCCEEDED(result), "Failed to create Depth Stancil Texture2D")
	//if (FAILED(result))
	//{
	//	return false;
	//}

	// Bind depth stencil state
	//myImmediateContext->OMSetDepthStencilState(pDSState, 1);




	// CREATE DEPTH STENCIL
	myDepthStencilView = TextureAssetHandler::CreateDepthStencil(L"Render Depth", myRenderWidth, myRenderHeight);

	//BIND RENDER TARGET VIEW
	myImmediateContext->OMSetRenderTargets(1, &myRenderTargetView, myDepthStencilView->myDSV.Get()); // depth stencil view is for shadow map


	D3D11_DEPTH_STENCIL_DESC depthDisabledStencilDesc;
	// Clear the second depth stencil state before setting the parameters.
	ZeroMemory(&depthDisabledStencilDesc, sizeof(depthDisabledStencilDesc));

	// Now create a second depth stencil state which turns off the Z buffer for 2D rendering.  The only difference is 
	// that DepthEnable is set to false, all other parameters are the same as the other depth stencil state.
	//depthDisabledStencilDesc.DepthEnable = false;
	//depthDisabledStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	//depthDisabledStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	//depthDisabledStencilDesc.StencilEnable = true;
	//depthDisabledStencilDesc.StencilReadMask = 0xFF;
	//depthDisabledStencilDesc.StencilWriteMask = 0xFF;
	//depthDisabledStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	//depthDisabledStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	//depthDisabledStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	//depthDisabledStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	//depthDisabledStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	//depthDisabledStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	//depthDisabledStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	//depthDisabledStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	//// Create the state using the device.
	//result = Device->CreateDepthStencilState(&depthDisabledStencilDesc, &myDepthDisabledStencilState);
	//GE_ASSERT(SUCCEEDED(result), "Failed to create Depth Stancil State")
	//if (FAILED(result))
	//{
	//	return false;
	//}

	if (!CreateRasterizerState())
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


	//D3D11_RASTERIZER_DESC rasterizerDesc = {};
	//rasterizerDesc.FillMode = D3D11_FILL_SOLID; // or D3D11_FILL_WIREFRAME for wireframe rendering
	//rasterizerDesc.CullMode = D3D11_CULL_NONE;  // or D3D11_CULL_NONE to disable backface culling
	//rasterizerDesc.FrontCounterClockwise = true; // Set to TRUE if your models use counterclockwise winding order for front faces
	//rasterizerDesc.DepthClipEnable = TRUE; // Enable depth clipping

	//
	//result = Device->CreateRasterizerState(&rasterizerDesc, &myFrontCulling);
	//GE_ASSERT(SUCCEEDED(result), "Failed to create Rasterizer State");
	//if (FAILED(result))
	//{
	//	return false;
	//}

	// Set the rasterizer state
	//myImmediateContext->RSSetState(myFrontCulling);

	
	//rasterizerDesc.FillMode = D3D11_FILL_SOLID; // or D3D11_FILL_WIREFRAME for wireframe rendering
	//rasterizerDesc.CullMode = D3D11_CULL_BACK;  // or D3D11_CULL_NONE to disable backface culling
	//rasterizerDesc.FrontCounterClockwise = FALSE; // Set to TRUE if your models use counterclockwise winding order for front faces
	//rasterizerDesc.DepthClipEnable = TRUE; // Enable depth clipping

	//
	//result = Device->CreateRasterizerState(&rasterizerDesc, &myBackCulling);
	//GE_ASSERT(SUCCEEDED(result), "Failed to create Rasterizer State");
	//if (FAILED(result))
	//{
	//	return false;
	//}
	// Set the rasterizer state
	//myImmediateContext->RSSetState(rasterizerState);


	if (!ResizeViewport())
	{
		return false;
	}


	if (!CreateShaderResourceView())
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

#if ENABLE_VR
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
	//TurnZBufferOn();

	SwapChain->Present(0, 0);
	myVRSystem.Update();

#if ENABLE_VR

	vr::Texture_t leftEyeTexture = { m_RenderTextureLeft->GetTexture(), vr::TextureType_DirectX, vr::ColorSpace_Auto };
	vr::EVRCompositorError error1 = vr::VRCompositor()->Submit(vr::Eye_Left, &leftEyeTexture);
	vr::Texture_t rightEyeTexture = { m_RenderTextureRight->GetTexture(), vr::TextureType_DirectX, vr::ColorSpace_Auto };
	vr::VRCompositor()->Submit(vr::Eye_Right, &rightEyeTexture);
	if (error1)
		printf("error is %d \n", error1);

#endif

}



Rect DX11::GetClientSize()
{
	RECT clientRect;
	GetClientRect(WindowHandle, &clientRect);


	Rect returnValue;
	returnValue.Left = clientRect.left;
	returnValue.Right = clientRect.right;
	returnValue.Bottom = clientRect.bottom;
	returnValue.Top = clientRect.top;
	

	return returnValue;
}

UINT DX11::GetScreenObjectId(const UINT x, const UINT y) const
{
	if (x >= IDBufferDesc.Width) return 0;
	if (y >= IDBufferDesc.Height) return 0;

	const D3D11_BOX b = { x, y, 0, x + 1, y + 1, 1 };
	GetContext()->CopySubresourceRegion(StagingTex.Get(), 0, 0, 0, 0, IDBufferTex.Get(), 0, &b);

	D3D11_MAPPED_SUBRESOURCE mapped;
	const HRESULT h = GetContext()->Map(StagingTex.Get(), 0, D3D11_MAP_READ, 0, &mapped);
	if (FAILED(h))
	{
		return 0;
	}
	const UINT32* p = static_cast<UINT32*>(mapped.pData);
	GetContext()->Unmap(StagingTex.Get(), 0);
	//const FLOAT clear[4] = { 0, 0, 0, 0 };
	//DX11::ourContext->ClearRenderTargetView(DX11::ourIDBuffer.Get(), clear);
	if (p == nullptr) return 0;
	return *p;
}

bool DX11::CreateSwapChain(bool aEnableDeviceDebug)
{

	UINT createDeviceFlags = 0;

#if _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	RECT clientRect = { 0,0,0,0 };
	GetClientRect(WindowHandle, &clientRect);

	if (myVRSystem.GetWidth() == 0)
	{
		myRenderWidth = clientRect.right - clientRect.left;
	}
	else
	{
		myRenderWidth = myVRSystem.GetWidth();
	}

	if (myVRSystem.GetHeight() == 0)
	{
		myRenderHeight = clientRect.bottom - clientRect.top;
	}
	else
	{
		myRenderHeight = myVRSystem.GetHeight();
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
	swapDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM; // unsigned normal
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

	HRESULT errorCode = S_FALSE;
	for (unsigned i = 0; i < numDriverTypes; ++i)
	{
		errorCode = D3D11CreateDeviceAndSwapChain(nullptr, driverTypes[i], nullptr, createDeviceFlags,
			featureLevels, numFeatureLevels, D3D11_SDK_VERSION, &swapDesc, &SwapChain, &Device,
			&featureLevel, &myImmediateContext);

		GE_ASSERT(SUCCEEDED(result), "Failed to create SwapChain and Device")
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

	return true;
}

bool DX11::CreateTexture2D()
{

	RECT clientRect = { 0,0,0,0 };
	GetClientRect(WindowHandle, &clientRect);

	HRESULT result = SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&BackBufferTex));
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
	RECT clientRect = { 0,0,0,0 };
	GetClientRect(WindowHandle, &clientRect);

	ComPtr<ID3D11Texture2D> depthBufferTexture;
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	ZeroMemory(&depthBufferDesc, sizeof(D3D11_TEXTURE2D_DESC));
	depthBufferDesc.Width = clientRect.right - clientRect.left;
	depthBufferDesc.Height = clientRect.bottom - clientRect.top;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;


	HRESULT result = Device->CreateTexture2D(&depthBufferDesc, nullptr, depthBufferTexture.GetAddressOf());
	if (FAILED(result))
		return false;

	result = Device->CreateDepthStencilView(depthBufferTexture.Get(), nullptr, DepthBuffer.GetAddressOf());
	if (FAILED(result))
		return false;

	return true;
}

bool DX11::ResizeViewport()
{
	//VIEWPORT CREATION
	myViewport.Width = static_cast<float>(myRenderWidth);
	myViewport.Height = static_cast<float>(myRenderHeight);
	myViewport.TopLeftX = 0;
	myViewport.TopLeftY = 0;
	myViewport.MinDepth = 0.0f;
	myViewport.MaxDepth = 1.0f;

	// BIND VIEWPORT
	myImmediateContext->RSSetViewports(1, &myViewport);

	return true;
}

bool DX11::CreateRasterizerState()
{

	D3D11_RASTERIZER_DESC rasterizerDesc;
	ZeroMemory(&rasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));
	rasterizerDesc.CullMode = D3D11_CULL_BACK;
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;

	ID3D11RasterizerState* rasterizerState;
	HRESULT result = Device->CreateRasterizerState(&rasterizerDesc, &rasterizerState);
	if (FAILED(result))
		return false;

	myImmediateContext->RSSetState(rasterizerState);

	return true;
}

bool DX11::CreateShaderResourceView()
{
#if ENABLE_VR

	if (m_RenderTextureLeft)
	{
		m_RenderTextureLeft->Shutdown();
	}
	if (m_RenderTextureRight)
	{
		m_RenderTextureRight->Shutdown();
	}

	// Create the render to texture object.
	m_RenderTextureLeft = MakeRef<RenderTexture>();
	if (!m_RenderTextureLeft)
	{
		return false;
	}

	// Initialize the render to texture object.
	m_RenderTextureLeft->SetName("VR Left Eye");
	result = m_RenderTextureLeft->Initialize(Device.Get(), myRenderWidth, myRenderHeight);
	GE_ASSERT(SUCCEEDED(result), "Failed to create Left Render Target (VR)");
	if (FAILED(result))
	{
		return false;
	}



	m_RenderTextureRight = MakeRef<RenderTexture>();
	if (!m_RenderTextureRight)
	{
		return false;
	}

	// Initialize the render to texture object.
	m_RenderTextureRight->SetName("VR Right Eye");

	result = m_RenderTextureRight->Initialize(Device.Get(), myRenderWidth, myRenderHeight);
	GE_ASSERT(SUCCEEDED(result), "Failed to create Right Render Target (VR)");
	if (FAILED(result))
	{
		return false;
	}
#endif


	
	if (myScreenView)
	{
		myScreenView->Shutdown();
	}

	// Create the render to texture object.
	myScreenView = MakeRef<RenderTexture>();
	if (!myScreenView)
	{
		return false;
	}

	//m_nRenderHeight = clientRect.bottom - clientRect.top;


	// Initialize the render to texture object.
	myScreenView->SetName("Window View");
	HRESULT result = myScreenView->Initialize(Device.Get(), static_cast<int>(myRenderWidth), static_cast<int>(myRenderHeight));
	GE_ASSERT(SUCCEEDED(result), "Failed to create Render Target (Flatscreen)")
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

bool DX11::CreateSampler()
{
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

	const HRESULT result = Device->CreateSamplerState(&samplerDesc, SampleStateDefault.GetAddressOf());
	if (FAILED(result))
		return false;

	GetContext()->PSSetSamplers(0, 1, SampleStateDefault.GetAddressOf());

	return true;
}

bool DX11::CreateSelectionView()
{
	RECT clientRect = { 0,0,0,0 };
	GetClientRect(WindowHandle, &clientRect);

	D3D11_TEXTURE2D_DESC textureDesc;
	ZeroMemory(&textureDesc, sizeof(D3D11_TEXTURE2D_DESC));

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

	HRESULT result = Device->CreateTexture2D(&textureDesc, nullptr, IDBufferTex.GetAddressOf());
	if (FAILED(result))
		return false;

	D3D11_RENDER_TARGET_VIEW_DESC idBufferViewDesc;
	ZeroMemory(&idBufferViewDesc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
	idBufferViewDesc.Format = IDBufferDesc.Format;
	idBufferViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	idBufferViewDesc.Texture2D.MipSlice = 0;

	result = Device->CreateRenderTargetView(IDBufferTex.Get(), &idBufferViewDesc, IDBuffer.GetAddressOf());
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
	result = Device->CreateTexture2D(&StagingTexDesc, nullptr, StagingTex.GetAddressOf());
	if (FAILED(result))
		return false;

	return true;
}

void DX11::Resize()
{
	myRenderWidth = 0;
	myRenderHeight = 0;


	// TODO : IF VR THEN GET RECOMMENDED SIZE

	RECT clientRect = { 0,0,0,0 };
	GetClientRect(WindowHandle, &clientRect);

	if (myVRSystem.GetWidth() == 0)
	{
		//myVRSystem.SetWidth(clientRect.right - clientRect.left);
		myRenderWidth = clientRect.right - clientRect.left;
	}

	if (myVRSystem.GetHeight() == 0)
	{
		//myVRSystem.SetHeight(clientRect.bottom - clientRect.top);
		myRenderHeight = clientRect.bottom - clientRect.top;
	}

	// Release the existing resources
	myImmediateContext->OMSetRenderTargets(0, nullptr, nullptr);
	SafeRelease(myRenderTargetView);
	//SafeRelease(myDepthStencilView);
	SafeRelease(myBackBufferTex);



	// Resize the swap chain
	HRESULT hr = SwapChain->ResizeBuffers(1, myRenderWidth, myRenderHeight, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
	if (FAILED(hr))
	{
		// Handle the error
		return;
	}
	// Create a new render target view
	hr = SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&myBackBufferTex));
	if (FAILED(hr))
	{
		// Handle the error
		return;
	}

	D3D11_TEXTURE2D_DESC BBDesc;
	ZeroMemory(&BBDesc, sizeof(D3D11_TEXTURE2D_DESC));
	myBackBufferTex->GetDesc(&BBDesc);

	D3D11_RENDER_TARGET_VIEW_DESC RTVDesc;
	ZeroMemory(&RTVDesc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
	RTVDesc.Format = BBDesc.Format;
	RTVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	RTVDesc.Texture2D.MipSlice = 0;

	hr = Device->CreateRenderTargetView(myBackBufferTex, &RTVDesc, &myRenderTargetView);
	SafeRelease(myBackBufferTex);
	if (FAILED(hr))
	{
		// Handle the error
		return;
	}

	myDepthStencilView = TextureAssetHandler::CreateDepthStencil(L"Render Depth", myRenderWidth, myRenderHeight);

	// Create a depth-stencil buffer with the same dimensions
	//ID3D11Texture2D* pDepthStencil = NULL;
	//D3D11_TEXTURE2D_DESC depthStencilDesc = {};
	//depthStencilDesc.Width = myRenderWidth;
	//depthStencilDesc.Height = myRenderHeight;
	//depthStencilDesc.MipLevels = 1;
	//depthStencilDesc.ArraySize = 1;
	//depthStencilDesc.Format = DXGI_FORMAT_D32_FLOAT;
	//depthStencilDesc.SampleDesc.Count = 1;
	//depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	//depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	//hr = Device->CreateTexture2D(&depthStencilDesc, nullptr, &pDepthStencil);
	//if (FAILED(hr))
	//{
	//	// Handle the error
	//	return;
	//}

	//D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	//ZeroMemory(&descDSV, sizeof(descDSV));
	//descDSV.Format = depthStencilDesc.Format;
	//descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	//descDSV.Texture2D.MipSlice = 0;

	//hr = Device->CreateDepthStencilView(pDepthStencil, &descDSV, myDepthStencilView->myDSV.GetAddressOf());
	//if (FAILED(hr))
	//{
	//	// Handle the error
	//	return;
	//}
	//SafeRelease(pDepthStencil);


	// Update the viewport with the new dimensions
	myViewport.Width = static_cast<float>(myRenderWidth);
	myViewport.Height = static_cast<float>(myRenderHeight);

	

	// Initialize the render to texture object.
	if (!CreateShaderResourceView())
	{
		return;
	}

	if (!ResizeViewport())
	{
		return;
	}
}

void DX11::ResetViewport()
{
	myViewport.Width = static_cast<float>(myRenderWidth);
	myViewport.Height = static_cast<float>(myRenderHeight);
	myImmediateContext->RSSetViewports(1, &myViewport);
}
