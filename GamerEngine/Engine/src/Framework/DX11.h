#pragma once
#include <array>
#include <wrl.h>



#include "GraphicsEngine.h"
#include "Render/DepthStencil.h"
#include "Render/RenderTexture.h"
#include "VR/VRSystem.h"



using namespace Microsoft::WRL;

namespace vr
{
	class IVRSystem;
	class IVRRenderModels;
	struct TrackedDevicePose_t;
}

struct ID3D11Device;
struct ID3D11DeviceContext;
struct IDXGISwapChain;
struct ID3D11SamplerState;
struct ID3D11RenderTargetView;
struct ID3D11DepthStencilView;
struct ID3D11Texture2D;
struct ID3D11ShaderResourceView;
struct D3D11_TEXTURE2D_DESC;



class DX11 
{

private:
	ComPtr<ID3D11Device> Device;
	ComPtr<ID3D11DeviceContext> Context;
	ComPtr<IDXGISwapChain> SwapChain;
	
	ComPtr<ID3D11SamplerState> SampleStateDefault;
	ComPtr<ID3D11SamplerState> SamplerStateWrap;
	
	ComPtr<ID3D11RenderTargetView> BackBuffer;
	ComPtr<ID3D11ShaderResourceView> BackBufferSRV;
	ID3D11Texture2D* BackBufferTex;
	ComPtr<ID3D11DepthStencilView> DepthBuffer;
	
	ComPtr<ID3D11ShaderResourceView> RenderSRV;
	ComPtr<ID3D11RenderTargetView> RenderRTV;
	
	ComPtr<ID3D11RenderTargetView> IDBuffer;
	ComPtr<ID3D11Texture2D> IDBufferTex;
	ComPtr<ID3D11Texture2D> StagingTex;
	D3D11_TEXTURE2D_DESC IDBufferDesc;
	D3D11_TEXTURE2D_DESC StagingTexDesc;

	// NEW

	D3D_FEATURE_LEVEL		featureLevel;
	D3D_DRIVER_TYPE			driverType;

	ID3D11Texture2D* myBackBufferTex;

	ID3D11RenderTargetView* myRenderTargetView;
	Ref<DepthStencil> myDepthStencilView;
	ID3D11ShaderResourceView* myDepthStencilSRV;

	D3D11_VIEWPORT myViewport;
	ID3D11DepthStencilState* pDSState;
	ID3D11DepthStencilState* myDepthDisabledStencilState;
	ID3D11DeviceContext* myImmediateContext;

	VRSystem myVRSystem;

	uint32_t myRenderWidth;
	uint32_t myRenderHeight;

	Ref<RenderTexture> m_RenderTextureLeft;
	Ref<RenderTexture> m_RenderTextureRight;
	Ref<RenderTexture> myScreenView;

	ID3D11RasterizerState* myFrontCulling;
	ID3D11RasterizerState* myBackCulling;

public:
	DX11();
	~DX11();
	ComPtr<IDXGISwapChain> GetSwapChain();
	ID3D11RasterizerState* GetFrontCulling() const;

	static DX11& Get();

	ID3D11Device* GetDevice() const;

	ID3D11DeviceContext* GetContext() const;

	Ref<DepthStencil> GetDepthStencilView();
	ID3D11ShaderResourceView* GetDepthSRV() const;

	VRSystem& GetVRSystem();

	static void TurnZBufferOn();
	static void TurnZBufferOff();

	Ref<RenderTexture>& GetLeftEyeView();
	Ref<RenderTexture>& GetRightEyeView();
	Ref<RenderTexture>& GetScreenView();
	ID3D11RenderTargetView* GetRenderTargetView() const;
	


	bool IsVrNull();

	void UpdateHMDMatrixPose();
	bool Init(HWND aWindowHandle, bool aEnableDeviceDebug, bool aEnabledVR = false);
	static void BeginFrame(std::array<float, 4> aClearColor = { 0.0f, 0.0f, 0.0f, 0.0f });
	void EndFrame();

	static RECT GetClientSize();

	void Resize();

	void ResetViewport();

	void ResetRenderTarget(bool isUsingEditor = false, bool useDepth = true);

	Vector2ui GetScreenSize() const;


	UINT GetScreenObjectId(UINT x, UINT y) const;
	static bool ResizeViewport();

private:

	
	bool CreateSwapChain(bool aEnableDeviceDebug);
	bool CreateTexture2D();
	bool CreateDepthBuffer();
	bool CreateShaderResourceView();
	bool CreateSampler();
	bool CreateSelectionView();

	inline static HWND WindowHandle;
};


