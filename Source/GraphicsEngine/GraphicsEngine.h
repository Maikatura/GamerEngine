#pragma once
#include <memory>


class DX11;
class Scene;
class ForwardRenderer;
class ModelAssetHandler;

class GraphicsEngine
{
	SIZE myWindowSize{0,0};
	HWND myWindowHandle{};

	// Container window message pump.
	static LRESULT CALLBACK WinProc(_In_ HWND hWnd, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam);

public:

	bool Initialize(unsigned someX, unsigned someY, unsigned someWidth, unsigned someHeight, bool enableDeviceDebug);

	void BeginFrame();
	void EndFrame();
	void RenderFrame();

	std::shared_ptr<Scene> myScene;
	std::shared_ptr<ForwardRenderer> myForwardRenderer;
	std::shared_ptr<ModelAssetHandler> myAssetHandler;

	[[nodiscard]] HWND FORCEINLINE GetWindowHandle() const { return myWindowHandle; }
	[[nodiscard]] SIZE FORCEINLINE GetWindowSize() const { return myWindowSize; }
};

