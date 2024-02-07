#pragma once
#include <functional>
#include <memory>
#include <Utilites/InputManager.h>
#include <vector>
#include <Math/MathTypes.hpp>
#include <Types/EnumTypes.h>
#include <thread>
#include <functional>

#include "openvr.h"
#include "Utilites/Pointers.h"

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


	inline static GraphicsEngine* myInstance;

	RenderMode myRenderMode = RenderMode::Default;


	Ref<ForwardRenderer> myForwardRenderer;
	Ref<DeferredRenderer> myDeferredRenderer;
	Ref<ShadowRenderer> myShadowRenderer;
	Ref<PostProcessRenderer> myPostProcessRenderer;
	Ref<GBuffer> myGBuffer;

	Ref<DropManager> myDropManager;
	Ref<std::thread> myUpdateThread = nullptr;

	bool myUpdateCondition = false;
	bool myUseEditor = true;
	bool myWantToResizeBuffers = false;
	bool myIsMinimized = false;
	bool myIsPaused = false;
	bool myIsRunning = true;

	Vector2ui myEditorWindowSize;

public:

	static GraphicsEngine* Get();

	~GraphicsEngine();

	bool Initialize(unsigned someX, unsigned someY, unsigned someWidth, unsigned someHeight, bool enableDeviceDebug, const std::wstring& aName = L"GamerEngine", bool aBoolToUseEditor = false, bool isVRMode = false);

	void BeginFrame();
	void EndFrame() const;
	void OnFrameUpdate(bool aShouldRunLoop);
	void RenderScene(VREye anEye) const;
	void OnFrameRender() const;

	void StartUpdateThread();
	static void StopUpdateThread();

	void SetMinimized(bool aMinimizedState);
	void SetUpdateBuffers(bool aUpdate);

	void SetWinProc(const std::function<bool(HWND, UINT, WPARAM, LPARAM)>& aFunction);

	void SetRenderMode(RenderMode aRenderMode) { myRenderMode = aRenderMode; }
	RenderMode GetRenderMode() const { return myRenderMode; }
	int GetRenderModeInt() const { return (int)myRenderMode; }

	static void ResetStates();
	

	std::vector<std::string> GetDropPath() const;

	bool GetEngineUpdateRuntime() const;
	void SetEngineUpdateRuntime(bool aCondition);

	bool GetPauseState() const;
	void SetPauseState(bool aCondition);
	bool GetEditorMode() const;

	bool GetEngineRunning() const;
	void SetEngineRunning(bool aCondition);
	int GetRenderPass() const;

	Vector2ui GetEditorWindowSize() const;
	void SetEditorWindowSize(Vector2ui aEditorWindowSize);

	[[nodiscard]] HWND FORCEINLINE GetWindowHandle() const { return myWindowHandle; }
	[[nodiscard]] SIZE FORCEINLINE GetWindowSize() const { return myWindowSize; }
	[[nodiscard]] void FORCEINLINE SetWindowSize(int aX, int aY) { myWindowSize.cx = aX; myWindowSize.cy = aY; }
	

private:

	int myRenderPass = 0;
	
	
};

