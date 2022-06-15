#pragma once
#include <array>
#include <wrl.h>

using namespace Microsoft::WRL;

struct ID3D11Device;
struct ID3D11DeviceContext;
struct IDXGISwapChain;
struct ID3D11SamplerState;
struct ID3D11RenderTargetView;
struct ID3D11DepthStencilView;

class DX11 
{

public:
	DX11();
	~DX11();

	static bool Init(HWND aWindowHandle, bool aEnableDeviceDebug);
	static void BeginFrame(std::array<float, 4> aClearColor = { 0.0f, 0.0f, 0.0f, 0.0f });
	static void EndFrame();

	static ComPtr<ID3D11Device> Device;
	static ComPtr<ID3D11DeviceContext> Context;
	static ComPtr<IDXGISwapChain> SwapChain;

	static ComPtr<ID3D11SamplerState> SampleStateDefault;
	static ComPtr<ID3D11SamplerState> SamplerStateWrap;

	static ComPtr<ID3D11RenderTargetView> BackBuffer;
	static ComPtr<ID3D11DepthStencilView> DepthBuffer;
};
