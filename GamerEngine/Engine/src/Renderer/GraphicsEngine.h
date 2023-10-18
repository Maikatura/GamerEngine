#pragma once
#include <functional>
#include <memory>
#include <Utilites/InputManager.h>
#include <vector>
#include <Math/Vector4.hpp>
#include <Renderer/Types/EnumTypes.h>
#include <thread>
#include <functional>

#include "openvr.h"

enum class VREye;
class DX11;
class ForwardRenderer;
class DeferredRenderer;
class ShadowRenderer;
class PostProcessRenderer;
class GBuffer;
class Scene;
class ModelAssetHandler;
class DropManager;

class GraphicsEngine
{
	SIZE myWindowSize{0,0};
	HWND myWindowHandle{};

	// Container window message pump.
	static LRESULT CALLBACK WinProc(_In_ HWND hWnd, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam);

	std::function<bool(HWND, UINT, WPARAM, LPARAM)> myWinProcFunction;

	std::shared_ptr<CommonUtilities::InputManager> myInputManager;
	std::shared_ptr<DropManager> myDropManager;

	inline static GraphicsEngine* myInstance;

	RenderMode myRenderMode = RenderMode::Default;

	std::shared_ptr<std::thread> myUpdateThread = nullptr;

	bool myUpdateCondition = false;
	bool myUseEditor = true;
	bool myWantToResizeBuffers = false;
	bool myIsMinimized = false;
	bool myIsPaused = false;
	bool myIsRunning = true;

public:

	static GraphicsEngine* Get();

	~GraphicsEngine();

	bool Initialize(unsigned someX, unsigned someY, unsigned someWidth, unsigned someHeight, bool enableDeviceDebug, std::wstring aName = L"GamerEngine", bool aBoolToUseEditor = false, bool isVRMode = false);
	void UpdateHMDMatrixPose();

	void BeginFrame();
	void EndFrame();
	void OnFrameUpdate(bool aShouldRunLoop);
	void RenderScene(VREye anEye);
	void OnFrameRender();

	void StartUpdateThread();
	void StopUpdateThread();

	void SetMinimized(bool aMinimizedState);
	void SetUpdateBuffers(bool aUpdate);

	void SetWinProc(std::function<bool(HWND, UINT, WPARAM, LPARAM)> aFunction);

	void SetRenderMode(RenderMode aRenderMode) { myRenderMode = aRenderMode; }
	RenderMode GetRenderMode() const { return myRenderMode; }
	int GetRenderModeInt() const { return (int)myRenderMode; }

	void ResetStates() const;
	
	std::shared_ptr<CommonUtilities::InputManager> GetInput();
	std::vector<std::string> GetDropPath();

	bool GetEngineUpdateRuntime();
	void SetEngineUpdateRuntime(bool aCondition);

	bool GetPauseState();
	void SetPauseState(bool aCondition);

	bool GetEngineRunning();
	void SetEngineRunning(bool aCondition);
	int GetRenderPass();

	std::shared_ptr<ForwardRenderer> myForwardRenderer;
	std::shared_ptr<DeferredRenderer> myDeferredRenderer;
	std::shared_ptr<ShadowRenderer> myShadowRenderer;
	std::shared_ptr<PostProcessRenderer> myPostProcessRenderer;
	std::shared_ptr<GBuffer> myGBuffer;

	[[nodiscard]] HWND FORCEINLINE GetWindowHandle() const { return myWindowHandle; }
	[[nodiscard]] SIZE FORCEINLINE GetWindowSize() const { return myWindowSize; }
	[[nodiscard]] void FORCEINLINE SetWindowSize(int aX, int aY) { myWindowSize.cx = aX; myWindowSize.cy = aY; }


private:

	int myRenderPass = 0;
	int m_iTrackedControllerCount;
	int m_iTrackedControllerCount_Last;
	int m_iValidPoseCount;
	int m_iValidPoseCount_Last;
	bool m_bShowCubes;

	std::string m_strPoseClasses;                            // what classes we saw poses for this frame
	char m_rDevClassChar[vr::k_unMaxTrackedDeviceCount];   // for each device, a character representing its class
	
};

