#include "GraphicsEngine.pch.h"
#include "PostProcessRenderer.h"

#include "GraphicsEngine.h"
#include "Renderer.h"
#include "Input/Input.h"
#include "Utilites/StringCast.h"

bool PostProcessRenderer::Initialize()
{
	HRESULT result;
	D3D11_BUFFER_DESC bufferDescription = { 0 };
	bufferDescription.Usage = D3D11_USAGE_DYNAMIC;
	bufferDescription.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	bufferDescription.ByteWidth = sizeof(FrameBufferData);
	result = DX11::Get().GetDevice()->CreateBuffer(&bufferDescription, nullptr, myFrameBuffer.GetAddressOf());
	if(FAILED(result))
	{
		return false;
	}

	myFullscreenVS = TextureAssetHandler::GetVertexShader("Shaders\\Fullscreen_VS.cso");

	std::array<std::string, PP_COUNT> shaderPaths;

	shaderPaths[PP_COPY] = "PP-Copy_PS.cso";
	shaderPaths[PP_LUMINANCE] = "PP-Luminance_PS.cso";
	shaderPaths[PP_GAUSSIAN] = "PP-Gaussian_PS.cso";
	shaderPaths[PP_BLOOM] = "PP-Bloom_PS.cso";
	shaderPaths[PP_SSAO] = "PP-SSAO_PS.cso";
	shaderPaths[PP_TONEMAP] = "PP-Tonemap_PS.cso";

	for(unsigned int i = 0; i < PP_COUNT; i++)
	{
		myPassShaders[i] = TextureAssetHandler::GetPixelShader("Shaders\\" + shaderPaths[i]);
	}


	myNoiseTexture = TextureAssetHandler::GetTexture(L"Editor\\Textures\\BlueNoise.dds");

	ReInitialize();
	

	return true;
}

void PostProcessRenderer::ReInitialize()
{
	myHasInited = false;

	RECT clientRect = DX11::Get().GetClientSize();
	float width = static_cast<float>(clientRect.right - clientRect.left);
	float height = static_cast<float>(clientRect.bottom - clientRect.top);


	mySSAOTexture = std::make_shared<RenderTexture>();
	mySSAOTexture->Initialize(DX11::Get().GetDevice(), static_cast<int>(width), static_cast<int>(height), DXGI_FORMAT_R32_FLOAT);

	myFullSize = std::make_shared<RenderTexture>();
	myFullSize->Initialize(DX11::Get().GetDevice(), static_cast<int>(width), static_cast<int>(height));


	myHalfSize = std::make_shared<RenderTexture>();
	myHalfSize->Initialize(DX11::Get().GetDevice(), static_cast<int>(width * .5f), static_cast<int>(height * .5f));

	myQuarterSize = std::make_shared<RenderTexture>();
	myQuarterSize->Initialize(DX11::Get().GetDevice(), static_cast<int>(width * .25f), static_cast<int>(height * .25f));

	myBlur = std::make_shared<RenderTexture>();
	myBlur->Initialize(DX11::Get().GetDevice(), static_cast<int>(width * .25f), static_cast<int>(height * .25f));

	myNoiseTexture->SetAsResource(8);
}

std::shared_ptr<RenderTexture> PostProcessRenderer::CreateRenderTexture(const std::string& aName, float aWidth, float aHeight, DXGI_FORMAT aFormat)
{
	//if(auto i = myRenderTextures.find(aName); i == myRenderTextures.end())
	//{
	//	std::shared_ptr<RenderTexture> renderTex = std::make_shared<RenderTexture>();
	//	//HRESULT result;

	//	//renderTex->mySize = { static_cast<float>(aWidth), static_cast<float>(aHeight) };

	//	D3D11_TEXTURE2D_DESC desc = { 0 };
	//	desc.Width = static_cast<UINT>(aWidth);
	//	desc.Height = static_cast<UINT>(aHeight);
	//	desc.ArraySize = 1;
	//	desc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
	//	desc.SampleDesc.Count = 1;
	//	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	//	

	//	renderTex->Initialize(DX11::Get().GetDevice(), aWidth, aHeight);



	//	//DX11::Get().Device->CreateRenderTargetView(renderTex->GetTexture(), nullptr, renderTex-);
	//	//DX11::Get().Device->CreateShaderResourceView(renderTex->GetTexture(), nullptr, renderTex->GetShaderResourceView());
	//	//SafeRelease(renderTex.myTexture2D);

	//	myRenderTextures[aName] = renderTex;

	//	return myRenderTextures[aName];
	//}

	//return myRenderTextures[aName];

	return nullptr;
}

void PostProcessRenderer::Render(PostProcessPass aPass, Matrix4x4f aView, Matrix4x4f aProjection)
{
	if(!Renderer::GetCamera())
	{
		return;
	}


	if(myHasInited == false)
	{
		

		myHasInited = true;
	}

	/*ID3D11RenderTargetView* buffers[1]
	{
		DX11::Get().RenderRTV.Get()
	};*/

	switch(aPass)
	{
		case PP_COPY: break;
		case PP_LUMINANCE: break;
		case PP_GAUSSIAN: break;
		case PP_BLOOM:
		{
			
			SetDepthStencilState(DepthStencilState::Disabled);
			SetBlendState(BlendState::Additive);

			RenderTextureOnSlot(0, 0, PostProcessPass::PP_LUMINANCE, myFullSize);

			RenderTextureOnSlot(0, 0, PostProcessPass::PP_LUMINANCE, myHalfSize);

			RenderTextureOnSlot(0, 0, PostProcessPass::PP_COPY, myQuarterSize);

			RenderTextureOnSlot(0, 1, PostProcessPass::PP_GAUSSIAN, myBlur);

			RenderTextureOnSlot(0, 0, PostProcessPass::PP_COPY, myHalfSize);

			RenderTextureOnSlot(0, 0, PostProcessPass::PP_COPY, myFullSize);
			

			RenderTextureOnSlot(0,0, PostProcessPass::PP_BLOOM, myFullSize);

			/*myFullSize->Clear();
			myFullSize->SetAsTarget();
			DX11::Get().GetContext()->OMSetRenderTargets(1, &buffers[0], DX11::Get().DepthBuffer.Get());
			myBlur->SetAsResource(0);
			RenderPass(PostProcessPass::PP_COPY);

			DX11::Get().GetContext()->OMSetRenderTargets(1, &buffers[0], DX11::Get().DepthBuffer.Get());
			myFullSize->SetAsResource(0);
			myBlur->SetAsResource(1);
			RenderPass(PostProcessPass::PP_BLOOM);*/


			break;
		}

		case PP_SSAO:
		{
			myNoiseTexture->SetAsResource(9);

			HRESULT result = S_FALSE;
			D3D11_MAPPED_SUBRESOURCE bufferData;
			 
			const Vector2ui Resolution = {
				static_cast<unsigned int>(DX11::Get().GetScreenSize().x),
				static_cast<unsigned int>(DX11::Get().GetScreenSize().y)
			};

			myFrameBufferData.View = Matrix4x4f::GetFastInverse(aView);
			myFrameBufferData.CamTranslation = aView.GetPosition();
			myFrameBufferData.Projection = aProjection;
			myFrameBufferData.RenderMode = 0;

			myFrameBufferData.Resolution = Resolution;
			myFrameBufferData.FarPlane = Renderer::GetCamera()->myFarPlane;
			myFrameBufferData.NearPlane = Renderer::GetCamera()->myNearPlane;

			myFrameBufferData.DeltaTime = 0;
			myFrameBufferData.TotalTime = 0;

			myFrustum = CreateFrustumFromCamera(aView, Renderer::GetCamera()->GetVerticalFoV(), Renderer::GetCamera()->GetHorizontalFoV(), Renderer::GetCamera()->myNearPlane, Renderer::GetCamera()->myFarPlane);
			myFrameBufferData.FrustrumCorners[0] = myFrustum.NearTopLeft;
			myFrameBufferData.FrustrumCorners[1] = myFrustum.NearTopRight;
			myFrameBufferData.FrustrumCorners[2] = myFrustum.NearBottomLeft;
			myFrameBufferData.FrustrumCorners[3] = myFrustum.NearBottomRight;

			ZeroMemory(&bufferData, sizeof(D3D11_MAPPED_SUBRESOURCE));
			result = DX11::Get().GetContext()->Map(myFrameBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferData);
			if(FAILED(result))
			{
				// BOOM?
				return;
			}

			memcpy(bufferData.pData, &myFrameBufferData, sizeof(FrameBufferData));
			DX11::Get().GetContext()->Unmap(myFrameBuffer.Get(), 0);

			DX11::Get().GetContext()->VSSetConstantBuffers(0, 1, myFrameBuffer.GetAddressOf());
			DX11::Get().GetContext()->PSSetConstantBuffers(0, 1, myFrameBuffer.GetAddressOf());

			
			SetDepthStencilState(DepthStencilState::ReadWrite);
			SetBlendState(BlendState::Additive);

			
			RenderTextureOnSlot(8, 8, PostProcessPass::PP_SSAO, mySSAOTexture, DX11::Get().GetDepthStencilView());
			break;
		}

		case PP_TONEMAP:
		{
			ID3D11ShaderResourceView* nullsrv = nullptr;

			SetDepthStencilState(DepthStencilState::Disabled);
			SetBlendState(BlendState::Additive);

			
			RenderTextureOnSlot(0,0, PostProcessPass::PP_TONEMAP, myFullSize);
			RenderTextureOnSlot(0,0, PostProcessPass::PP_COPY, myFullSize);

			DX11::Get().ResetRenderTarget(GraphicsEngine::Get()->GetEditorMode());
			break;
		}
		case PP_COUNT: break;
		default:;
	}



}

void PostProcessRenderer::RenderTextureOnSlot(int aSlot,int aResourceSlot, PostProcessPass aPass, std::shared_ptr<RenderTexture> aRenderTexture, ID3D11DepthStencilView* depthStencilView)
{
	ID3D11ShaderResourceView* nullsrv = nullptr;

	aRenderTexture->ClearRenderTarget(DX11::Get().GetContext(), depthStencilView, 0.0f, 0.0f, 0.0f, 0.0f);
	DX11::Get().GetContext()->PSSetShaderResources(aSlot, 1, &nullsrv);


	

	aRenderTexture->SetRenderTarget(DX11::Get().GetContext(), depthStencilView);

	

	RenderPass(aPass);

	DX11::Get().ResetRenderTarget(GraphicsEngine::Get()->GetEditorMode());
	aRenderTexture->SetAsResource(DX11::Get().GetContext(), aResourceSlot);
}

void PostProcessRenderer::RenderPass(PostProcessPass aPass)
{
	HRESULT result = S_FALSE;

	DX11::Get().GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	DX11::Get().GetContext()->IASetInputLayout(nullptr);
	DX11::Get().GetContext()->IASetVertexBuffers(0, 0, nullptr, nullptr, nullptr);
	DX11::Get().GetContext()->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);
	DX11::Get().GetContext()->VSSetShader(myFullscreenVS.Get(), nullptr, 0);
	DX11::Get().GetContext()->PSSetShader(myPassShaders[aPass].Get(), nullptr, 0);
	DX11::Get().GetContext()->GSSetShader(nullptr, nullptr, 0);
	DX11::Get().GetContext()->Draw(3, 0);
}

void PostProcessRenderer::Release()
{
	
}

void PostProcessRenderer::ClearTargets()
{
	mySSAOTexture->ClearRenderTarget(DX11::Get().GetContext(), nullptr, 0.0f, 0.0f, 0.0f, 0.0f);
}
