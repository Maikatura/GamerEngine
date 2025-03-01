////////////////////////////////////////////////////////////////////////////////
// Filename: rendertextureclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _RENDERTEXTURECLASS_H_
#define _RENDERTEXTURECLASS_H_


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>


////////////////////////////////////////////////////////////////////////////////
// Class name: RenderTextureClass
////////////////////////////////////////////////////////////////////////////////
class RenderTexture
{
public:
	RenderTexture();
	RenderTexture(const RenderTexture&);
	~RenderTexture();

	bool Initialize(ID3D11Device*, int, int, DXGI_FORMAT aFormat = DXGI_FORMAT_R8G8B8A8_UNORM, const std::string& aRenderTextureName = "");
	void Shutdown();

	void SetName(const std::string& aRenderTextureName);
	const std::string& GetName();

	void SetRenderTarget(ID3D11DeviceContext*, ID3D11DepthStencilView*);
	void ClearRenderTarget(ID3D11DeviceContext*, ID3D11DepthStencilView*, float aRed = 0.5f, float aGreen = 0.5f, float aBlue = 0.5f, float aAlpha = 1.0f);
	void ClearRenderTarget(ID3D11DeviceContext*, ID3D11DepthStencilView*, const Vector4f& aColor = { 0.5f, 0.5f, 0.5f, 1.0f});
	ID3D11RenderTargetView* GetRenderTargetView();
	ID3D11ShaderResourceView* GetShaderResourceView();
	ID3D11Texture2D* GetTexture();

	void SetAsResource(ID3D11DeviceContext* deviceContext, int aSlot);

private:
	std::string myRenderTextureName;
	ID3D11Texture2D* m_renderTargetTexture;
	ID3D11RenderTargetView* m_renderTargetView;
	ID3D11ShaderResourceView* m_shaderResourceView;
};

#endif