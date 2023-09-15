#pragma once
#include <array>
#include <wrl.h>

#include <openvr.h>

#include "Renderer/Render/RenderTexture.h"

//#define VR_DISABLED

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

enum class VREye
{
	None,
	Right,
	Left
};

class DX11 
{

public:
	static ComPtr<ID3D11Device> Device;
	static ComPtr<ID3D11DeviceContext> Context;
	static ComPtr<IDXGISwapChain> SwapChain;

	static ComPtr<ID3D11SamplerState> SampleStateDefault;
	static ComPtr<ID3D11SamplerState> SamplerStateWrap;

	static ComPtr<ID3D11RenderTargetView> BackBuffer;
	static ComPtr<ID3D11ShaderResourceView> BackBufferSRV;
	static ID3D11Texture2D* BackBufferTex;
	static ComPtr<ID3D11DepthStencilView> DepthBuffer;

	static ComPtr<ID3D11ShaderResourceView> RenderSRV;
	static ComPtr<ID3D11RenderTargetView> RenderRTV;

	static ComPtr<ID3D11RenderTargetView> IDBuffer;
	static ComPtr<ID3D11Texture2D> IDBufferTex;
	static ComPtr<ID3D11Texture2D> StagingTex;
	static D3D11_TEXTURE2D_DESC IDBufferDesc;
	static D3D11_TEXTURE2D_DESC StagingTexDesc;

	// NEW

	static D3D_FEATURE_LEVEL		featureLevel;
	static D3D_DRIVER_TYPE			driverType;

	static ID3D11Texture2D* myBackBufferTex;

	static ID3D11RenderTargetView* myRenderTargetView;
	static ID3D11DepthStencilView* myDepthStencilView;

	static D3D11_VIEWPORT myViewport;
	static ID3D11DepthStencilState* pDSState;
	static ID3D11DepthStencilState* myDepthDisabledStencilState;
	static ID3D11DeviceContext* myImmediateContext;

	static vr::IVRSystem* m_pHMD;
	static vr::IVRRenderModels* m_pRenderModels;
	static vr::TrackedDevicePose_t m_rTrackedDevicePose[vr::k_unMaxTrackedDeviceCount];
	static Matrix4x4f m_rmat4DevicePose[vr::k_unMaxTrackedDeviceCount];
	static Matrix4x4f m_mat4HMDPose;
	static uint32_t m_nRenderWidth;
	static uint32_t m_nRenderHeight;

	static std::shared_ptr<RenderTexture> m_RenderTextureLeft;
	static std::shared_ptr<RenderTexture> m_RenderTextureRight;
	static std::shared_ptr<RenderTexture> myScreenView;

	static ID3D11RasterizerState* myFrontCulling;
	static ID3D11RasterizerState* myBackCulling;

	DX11();
	~DX11();
	static ID3D11Device* GetDevice();

	static ID3D11DeviceContext* GetContext();

	static ID3D11DepthStencilView* GetDepthStencilView();


	static void TurnZBufferOn();


	static void TurnZBufferOff();

	static void UpdateHMDMatrixPose();
	static bool Init(HWND aWindowHandle, bool aEnableDeviceDebug, bool aEnabledVR = false);
	static void BeginFrame(std::array<float, 4> aClearColor = { 0.0f, 0.0f, 0.0f, 0.0f });
	static void EndFrame();

	static RECT GetClientSize();

	static void Resize();

	static UINT GetScreenObjectId(UINT x, UINT y);
	static bool ResizeViewport();

private:

	
	static bool CreateSwapChain(bool aEnableDeviceDebug);
	static bool CreateTexture2D();
	static bool CreateDepthBuffer();
	static bool CreateShaderResourceView();
	static bool CreateSampler();
	static bool CreateSelectionView();

	inline static HWND WindowHandle;
};


