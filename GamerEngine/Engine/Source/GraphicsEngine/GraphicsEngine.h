#pragma once
#include <memory>
#include <InputManager.h>
#include <vector>
#include <Math/Vector4.hpp>


class DX11;
class ForwardRenderer;
class Scene;
class ModelAssetHandler;

class GraphicsEngine
{
	SIZE myWindowSize{0,0};
	HWND myWindowHandle{};

	// Container window message pump.
	static LRESULT CALLBACK WinProc(_In_ HWND hWnd, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam);

public:

	static GraphicsEngine* Get();

	~GraphicsEngine();

	bool Initialize(unsigned someX, unsigned someY, unsigned someWidth, unsigned someHeight, bool enableDeviceDebug, std::wstring aName = L"GamerEngine");

	void BeginFrame();
	void EndFrame();
	void RenderFrame();


	void SetMinimized(bool aMinimizedState);
	void SetUpdateBuffers(bool aUpdate);


	void SetColor(CommonUtilities::Vector4<float> aClearColor);
	CommonUtilities::Vector4<float> GetClearColor();

	std::shared_ptr<Scene> GetScene();
	std::shared_ptr<CommonUtilities::InputManager> GetInput();


	std::shared_ptr<Scene> myScene;
	std::shared_ptr<ForwardRenderer> myForwardRenderer;



	[[nodiscard]] HWND FORCEINLINE GetWindowHandle() const { return myWindowHandle; }
	[[nodiscard]] SIZE FORCEINLINE GetWindowSize() const { return myWindowSize; }
	[[nodiscard]] void FORCEINLINE SetWindowSize(int aX, int aY) { myWindowSize.cx = aX; myWindowSize.cy = aY; }
private:
	std::shared_ptr<CommonUtilities::InputManager> myInputManager;

	CommonUtilities::Vector4<float> myClearColor;

	inline static GraphicsEngine* myInstance;
	bool myUpdateBuffers;
	bool myIsMinimized;
};

