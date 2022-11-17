#pragma once
#include <memory>
#include <InputManager.h>
#include <vector>
#include <Math/Vector4.hpp>
#include <Types/EnumTypes.h>

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

public:

	static GraphicsEngine* Get();

	~GraphicsEngine();

	bool Initialize(unsigned someX, unsigned someY, unsigned someWidth, unsigned someHeight, bool enableDeviceDebug, std::wstring aName = L"GamerEngine", bool aBoolToUseEditor = false);

	void BeginFrame();
	void EndFrame();
	void OnFrameUpdate(bool aShouldRunLoop);
	void OnFrameRender();


	void SetMinimized(bool aMinimizedState);
	void SetUpdateBuffers(bool aUpdate);

	bool UseEditor() const;
	

	void SetRenderMode(RenderMode aRenderMode) { myRenderMode = aRenderMode; }
	RenderMode GetRenderMode() const { return myRenderMode; }


	void ResetStates() const;

	std::shared_ptr<Scene> GetScene();
	std::shared_ptr<CommonUtilities::InputManager> GetInput();
	std::vector<std::string> GetDropPath();

	bool GetEngineUpdateRuntime();
	void SetEngineUpdateRuntime(bool aCondition);

	std::shared_ptr<Scene> myScene;
	std::shared_ptr<ForwardRenderer> myForwardRenderer;
	std::shared_ptr<DeferredRenderer> myDeferredRenderer;
	std::shared_ptr<ShadowRenderer> myShadowRenderer;
	std::shared_ptr<PostProcessRenderer> myPostProcessRenderer;
	std::shared_ptr<GBuffer> myGBuffer;

	[[nodiscard]] HWND FORCEINLINE GetWindowHandle() const { return myWindowHandle; }
	[[nodiscard]] SIZE FORCEINLINE GetWindowSize() const { return myWindowSize; }
	[[nodiscard]] void FORCEINLINE SetWindowSize(int aX, int aY) { myWindowSize.cx = aX; myWindowSize.cy = aY; }


private:
	std::shared_ptr<CommonUtilities::InputManager> myInputManager;
	std::shared_ptr<DropManager> myDropManager;

	inline static GraphicsEngine* myInstance;

	RenderMode myRenderMode = RenderMode::Default;


	bool myUpdateCondition = false;
	bool myUseEditor = true;
	bool myWantToResizeBuffers = false;
	bool myIsMinimized = false;
};

