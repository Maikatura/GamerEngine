#include "GraphicsEngine.pch.h"
#include "PostProcessRenderer.h"

#include "GraphicsEngine.h"
#include "Renderer.h"
#include "Input/Input.h"

bool PostProcessRenderer::Initialize()
{
	HRESULT result;
	D3D11_BUFFER_DESC bufferDescription = { 0 };
	bufferDescription.Usage = D3D11_USAGE_DYNAMIC;
	bufferDescription.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	bufferDescription.ByteWidth = sizeof(FrameBufferData);
	result = DX11::Device->CreateBuffer(&bufferDescription, nullptr, myFrameBuffer.GetAddressOf());
	if(FAILED(result))
	{
		return false;
	}

	myFullscreenVS = TextureAssetHandler::GetVertexShader("Shaders\\FullscreenVS.cso");

	std::array<std::string, PP_COUNT> shaderPaths;

	shaderPaths[PP_COPY] = "PP-Copy.cso";
	shaderPaths[PP_LUMINANCE] = "PP-Luminance.cso";
	shaderPaths[PP_GAUSSIAN] = "PP-Gaussian.cso";
	shaderPaths[PP_BLOOM] = "PP-Bloom.cso";
	shaderPaths[PP_SSAO] = "PP-SSAO.cso";
	shaderPaths[PP_TONEMAP] = "PP-Tonemap.cso";

	for(unsigned int i = 0; i < PP_COUNT; i++)
	{
		myPassShaders[i] = TextureAssetHandler::GetPixelShader("Shaders\\PostProcess\\" + shaderPaths[i]);
	}




	myNoiseTexture = TextureAssetHandler::GetTexture(L"Editor\\Textures\\BlueNoise.dds");

	ReInitialize();
	

	return true;
}

void PostProcessRenderer::ReInitialize()
{
	myHasInited = false;

	myRenderTextures.clear();

	SetSamplerState(SamplerState::SS_Default, 0);
	SetSamplerState(SamplerState::SS_Wrap, 1);
	SetSamplerState(SamplerState::SS_PointClamp, 2);
	SetSamplerState(SamplerState::SS_PointClamp, 3);
	myNoiseTexture->SetAsResource(8);
}

PostProcessRenderer::RenderTexture PostProcessRenderer::CreateRenderTexture(std::string aName, UINT aWidth, UINT aHeight, DXGI_FORMAT aFormat)
{
	if(auto i = myRenderTextures.find(aName); i == myRenderTextures.end())
	{
		RenderTexture renderTex;
		HRESULT result;

		renderTex.mySize = { static_cast<float>(aWidth), static_cast<float>(aHeight) };

		D3D11_TEXTURE2D_DESC depthBufferDesc = { 0 };
		depthBufferDesc.Width = aWidth;
		depthBufferDesc.Height = aHeight;
		depthBufferDesc.ArraySize = 1;
		depthBufferDesc.Format = DXGI_FORMAT_D32_FLOAT;
		depthBufferDesc.SampleDesc.Count = 1;
		depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

		DX11::Device->CreateTexture2D(&depthBufferDesc, nullptr, &renderTex.myTexture2D);
		DX11::Device->CreateDepthStencilView(renderTex.myTexture2D, nullptr, renderTex.myDSV.GetAddressOf());
		SafeRelease(renderTex.myTexture2D);

		D3D11_TEXTURE2D_DESC bufferDesc = { 0 };
		bufferDesc.Width = static_cast<UINT>(aWidth);
		bufferDesc.Height = static_cast<UINT>(aHeight);
		bufferDesc.ArraySize = 1;
		bufferDesc.MipLevels = 1;
		bufferDesc.SampleDesc.Count = 1;
		bufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		bufferDesc.Format = aFormat;

		DX11::Device->CreateTexture2D(&bufferDesc, nullptr, &renderTex.myTexture2D);
		DX11::Device->CreateRenderTargetView(renderTex.myTexture2D, nullptr, renderTex.myRTV.GetAddressOf());
		DX11::Device->CreateShaderResourceView(renderTex.myTexture2D, nullptr, renderTex.mySRV.GetAddressOf());
		SafeRelease(renderTex.myTexture2D);

		myRenderTextures[aName] = renderTex;

		return myRenderTextures[aName];
	}

	return myRenderTextures[aName];
}

void PostProcessRenderer::Render(PostProcessPass aPass)
{
	if (!Renderer::GetCamera())
	{
		return;
	}


	if (myHasInited == false)
	{
		RECT clientRect = DX11::GetClientSize();
		UINT width = static_cast<UINT>(clientRect.right - clientRect.left);
		UINT height = static_cast<UINT>(clientRect.bottom - clientRect.top);


		mySSAOTexture = CreateRenderTexture("PP_SSAO", width, height, DXGI_FORMAT_R32_FLOAT);
		myFullSize = CreateRenderTexture("PP_FullSize", width, height);
		myHalfSize = CreateRenderTexture("PP_HalfSize", width * .5f, height * .5f);
		myQuarterSize = CreateRenderTexture("PP_QuarterSize", width * .25f, height * .25f);
		myBlur = CreateRenderTexture("PP_Blur", width * .25f, height * .25f);
		myHasInited = true;
	}

	ID3D11RenderTargetView* buffers[1]
	{
		DX11::RenderRTV.Get()
	};

	switch(aPass)
	{
		case PP_COPY: break;
		case PP_LUMINANCE: break;
		case PP_GAUSSIAN: break;
		case PP_BLOOM:
		{
			ID3D11ShaderResourceView* nullsrv = nullptr;

			SetDepthStencilState(DepthStencilState::DSS_Ignore);
			SetBlendState(BlendState::AdditiveBlend);
			DX11::Context->PSSetShaderResources(0, 1, &nullsrv);
			myFullSize.Clear();
			myFullSize.SetAsTarget();
			DX11::Context->PSSetShaderResources(0, 1, DX11::RenderSRV.GetAddressOf());
			RenderPass(PostProcessPass::PP_LUMINANCE);

			DX11::Context->PSSetShaderResources(0, 1, &nullsrv);
			myHalfSize.Clear();
			myHalfSize.SetAsTarget();
			myFullSize.SetAsResource(0);
			RenderPass(PostProcessPass::PP_COPY);

			DX11::Context->PSSetShaderResources(0, 1, &nullsrv);
			myQuarterSize.Clear();
			myQuarterSize.SetAsTarget();
			myHalfSize.SetAsResource(0);
			RenderPass(PostProcessPass::PP_COPY);

			DX11::Context->PSSetShaderResources(0, 1, &nullsrv);
			myBlur.Clear();
			myBlur.SetAsTarget();
			myQuarterSize.SetAsResource(0);
			RenderPass(PostProcessPass::PP_GAUSSIAN);

			DX11::Context->PSSetShaderResources(0, 1, &nullsrv);
			myHalfSize.Clear();
			myHalfSize.SetAsTarget();
			myBlur.SetAsResource(0);
			RenderPass(PostProcessPass::PP_COPY);

			DX11::Context->PSSetShaderResources(0, 1, &nullsrv);
			myFullSize.Clear();
			myFullSize.SetAsTarget();
			DX11::Context->OMSetRenderTargets(1, &buffers[0], DX11::DepthBuffer.Get());
			myBlur.SetAsResource(0);
			RenderPass(PostProcessPass::PP_COPY);

			DX11::Context->PSSetShaderResources(0, 1, &nullsrv);
			DX11::Context->OMSetRenderTargets(1, &buffers[0], DX11::DepthBuffer.Get());
			myFullSize.SetAsResource(0);
			myBlur.SetAsResource(1);
			RenderPass(PostProcessPass::PP_BLOOM);


			break;
		}

		case PP_SSAO:
		{
			myNoiseTexture->SetAsResource(8);

			HRESULT result = S_FALSE;
			D3D11_MAPPED_SUBRESOURCE bufferData;
			RECT clientRect = DX11::GetClientSize();
			const Vector2ui Resolution = {
				static_cast<unsigned int>(clientRect.right - clientRect.left),
				static_cast<unsigned int>(clientRect.bottom - clientRect.top)
			};

			
			myFrameBufferData.NearPlane = Renderer::GetCamera()->myNearPlane;
			myFrameBufferData.FarPlane = Renderer::GetCamera()->myFarPlane;
			myFrameBufferData.Resolution = Resolution;
			myFrameBufferData.DeltaTime = 0;
			myFrameBufferData.TotalTime = 0;
			myFrameBufferData.View = Matrix4x4f::GetFastInverse(Renderer::GetViewMatrix());
			myFrameBufferData.CamTranslation = Renderer::GetViewMatrix().GetPosition();
			myFrameBufferData.Projection = Renderer::GetProjectionMatrix();
			myFrameBufferData.RenderMode = 0;

			const float aspectRatio = (float)myFrameBufferData.Resolution.x / (float)myFrameBufferData.Resolution.y;
			float farPlane = Renderer::GetCamera()->myFarPlane;
			float halfHeight = farPlane * tanf(0.5f * Renderer::GetCamera()->GetVerticalFoV());
			float halfWidth = aspectRatio * halfHeight;

			myFrameBufferData.FrustrumCorners[0] = Vector4f(-halfWidth, -halfHeight, farPlane, 0);
			myFrameBufferData.FrustrumCorners[1] = Vector4f(-halfWidth, halfHeight, farPlane, 0);
			myFrameBufferData.FrustrumCorners[2] = Vector4f(halfWidth, halfHeight, farPlane, 0);
			myFrameBufferData.FrustrumCorners[3] = Vector4f(halfWidth, -halfHeight, farPlane, 0);


			ZeroMemory(&bufferData, sizeof(D3D11_MAPPED_SUBRESOURCE));
			result = DX11::Context->Map(myFrameBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferData);
			if(FAILED(result))
			{
				// BOOM?
				return;
			}

			memcpy(bufferData.pData, &myFrameBufferData, sizeof(FrameBufferData));
			DX11::Context->Unmap(myFrameBuffer.Get(), 0);

			DX11::Context->VSSetConstantBuffers(0, 1, myFrameBuffer.GetAddressOf());
			DX11::Context->PSSetConstantBuffers(0, 1, myFrameBuffer.GetAddressOf());

			ID3D11ShaderResourceView* nullsrv = nullptr;

			SetDepthStencilState(DepthStencilState::DSS_Ignore);
			SetBlendState(BlendState::AlphaBlend);
			mySSAOTexture.Clear();
			DX11::Context->PSSetShaderResources(6, 1, &nullsrv);

			mySSAOTexture.SetAsTarget();
			RenderPass(PostProcessPass::PP_SSAO);

			DX11::Context->OMSetRenderTargets(1, &buffers[0], DX11::DepthBuffer.Get());
			mySSAOTexture.SetAsResource(6);

			break;
		}

		case PP_TONEMAP:
		{
			ID3D11ShaderResourceView* nullsrv = nullptr;

			SetDepthStencilState(DepthStencilState::DSS_Ignore);
			SetBlendState(BlendState::AdditiveBlend);
			DX11::Context->PSSetShaderResources(0, 1, &nullsrv);
			myFullSize.Clear();
			myFullSize.SetAsTarget();
			DX11::Context->PSSetShaderResources(0, 1, DX11::RenderSRV.GetAddressOf());
			RenderPass(PostProcessPass::PP_TONEMAP);

			DX11::Context->PSSetShaderResources(0, 1, &nullsrv);

			DX11::Context->OMSetRenderTargets(1, &buffers[0], DX11::DepthBuffer.Get());
			myFullSize.SetAsResource(0);
			RenderPass(PostProcessPass::PP_COPY);
			break;
		}
		case PP_COUNT: break;
		default:;
	}




}

void PostProcessRenderer::RenderPass(PostProcessPass aPass)
{
	HRESULT result = S_FALSE;

	DX11::Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	DX11::Context->IASetInputLayout(nullptr);
	DX11::Context->IASetVertexBuffers(0, 0, nullptr, nullptr, nullptr);
	DX11::Context->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);
	DX11::Context->VSSetShader(myFullscreenVS.Get(), nullptr, 0);
	DX11::Context->PSSetShader(myPassShaders[aPass].Get(), nullptr, 0);
	DX11::Context->GSSetShader(nullptr, nullptr, 0);
	DX11::Context->Draw(3, 0);
}

void PostProcessRenderer::Release()
{
	
}

void PostProcessRenderer::ClearTargets()
{
	mySSAOTexture.Clear();
}
