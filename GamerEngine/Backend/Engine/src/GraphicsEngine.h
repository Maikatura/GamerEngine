#pragma once
#include <functional>
#include <memory>
#include <vector>
#include <Math/MathTypes.hpp>
#include <Types/EnumTypes.h>
#include <thread>
#include <functional>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include "openvr.h"
#include "Core/RenderModule.h"
#include "Font/Font.h"
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


struct EngineSetting
{
	Vector4f myClearColor;

	Vector4f myBlendColor1;
	Vector4f myBlendColor2;

	std::string PresetPath1;
	std::string PresetPath2;

	Vector4f myCurrentBlendColor;

	float myBlendValue = 0.0f;

	bool myIsBlending = false;
	bool myUseConsole = false;

	Vector3f CameraPos;
	Vector3f CameraRot;
};

class GraphicsEngine
{
	SIZE myWindowSize{0,0};
	HWND myWindowHandle{};
	Ref<GamerEngine::Font> myFont;

	// Container window message pump.
	static LRESULT CALLBACK WinProc(_In_ HWND hWnd, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam);

	std::function<bool(HWND, UINT, WPARAM, LPARAM)> myWinProcFunction;


	inline static GraphicsEngine* myInstance;

	RenderMode myRenderMode = RenderMode::Default;

	Ref<DropManager> myDropManager;

	//std::atomic_bool myRenderIsDone;

	std::thread myUpdateThread;

	std::atomic_bool myUpdateReady = false;
	std::mutex myUpdateMutex;
	std::atomic_bool myRunning = true;
	std::condition_variable myRenderCondition;
	
	bool myUpdateShouldRun = true;

	bool myUpdateCondition = false;
	bool myUseEditor = true;
	bool myWantToResizeBuffers = false;
	bool myIsMinimized = false;
	bool myIsPaused = false;
	bool myIsRunning = true;

	Vector2ui myEditorWindowSize;

	std::vector<std::function<void()>> myMainThreadQueue;
	std::mutex myMainThreadQueueMutex;


	std::vector<Ref<RenderModule>> myRenderModules;

	EngineSetting myEngineSettings;

	
public:

	static GraphicsEngine* Get();

	~GraphicsEngine();

	bool Initialize(unsigned someX, unsigned someY, unsigned someWidth, unsigned someHeight, bool enableDeviceDebug, const std::wstring& aName = L"GamerEngine", bool aBoolToUseEditor = false, bool isVRMode = false);

	template<class T>
	void AddRenderModule();
	Ref<RenderModule> GetRenderModule(int aModuleIndex);
	void RemoveRenderModule(int aModuleIndex);
	
	void BeginFrame();
	void EndFrame() const;
	void OnFrameUpdate();
	void RenderScene(VREye anEye) const;
	void OnFrameRender();

	void StartUpdateThread();
	void StopUpdateThread();

	void SubmitToMainThread(const std::function<void()>& function);
	void ExecuteMainThreadQueue();


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

	Vector2ui GetEditorWindowSize() const;
	void SetEditorWindowSize(Vector2ui aEditorWindowSize);

	EngineSetting& GetEngineSettings()
	{
		return myEngineSettings;
	}



	[[nodiscard]] HWND FORCEINLINE GetWindowHandle() const { return myWindowHandle; }
	[[nodiscard]] SIZE FORCEINLINE GetWindowSize() const { return myWindowSize; }
	[[nodiscard]] void FORCEINLINE SetWindowSize(int aX, int aY) { myWindowSize.cx = aX; myWindowSize.cy = aY; }
	

	
	
};



