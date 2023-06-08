#include "GraphicsEngine.pch.h"
#include "PostProcessRenderer.h"

#include "Renderer/GraphicsEngine.h"
#include "Renderer.h"
#include "Renderer/Input/Input.h"
#include "StringCast.h"

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

	myRenderTextures.clear();
	myNoiseTexture->SetAsResource(8);
}

std::shared_ptr<RenderTexture> PostProcessRenderer::CreateRenderTexture(const std::string& aName, float aWidth, float aHeight, DXGI_FORMAT aFormat)
{
	if(auto i = myRenderTextures.find(aName); i == myRenderTextures.end())
	{
		std::shared_ptr<RenderTexture> renderTex = std::make_shared<RenderTexture>();
		//HRESULT result;

		renderTex->mySize = { static_cast<float>(aWidth), static_cast<float>(aHeight) };

		D3D11_TEXTURE2D_DESC desc = { 0 };
		desc.Width = static_cast<UINT>(aWidth);
		desc.Height = static_cast<UINT>(aHeight);
		desc.ArraySize = 1;
		desc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
		desc.SampleDesc.Count = 1;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		

		DX11::Device->CreateTexture2D(&desc, nullptr, renderTex->myTexture2D.GetAddressOf());
		renderTex->myDepthStencil = TextureAssetHandler::CreateDepthStencil(Helpers::string_cast<std::wstring>(aName), static_cast<int>(aWidth), static_cast<int>(aHeight));


		DX11::Device->CreateRenderTargetView(renderTex->myTexture2D.Get(), nullptr, renderTex->myRTV.ReleaseAndGetAddressOf());
		DX11::Device->CreateShaderResourceView(renderTex->myTexture2D.Get(), nullptr, renderTex->mySRV.ReleaseAndGetAddressOf());
		//SafeRelease(renderTex.myTexture2D);

		myRenderTextures[aName] = renderTex;

		return myRenderTextures[aName];
	}

	return myRenderTextures[aName];
}

void PostProcessRenderer::Render(PostProcessPass aPass)
{
	if(!Renderer::GetCamera())
	{
		return;
	}


	if(myHasInited == false)
	{
		RECT clientRect = DX11::GetClientSize();
		float width = static_cast<float>(clientRect.right - clientRect.left);
		float height = static_cast<float>(clientRect.bottom - clientRect.top);


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

			SetDepthStencilState(DepthStencilState::Disabled);
			SetBlendState(BlendState::Additive);
			DX11::Context->PSSetShaderResources(0, 1, &nullsrv);
			DX11::Context->VSSetShaderResources(0, 1, &nullsrv);
			myFullSize->Clear();
			myFullSize->SetAsTarget();
			DX11::Context->PSSetShaderResources(0, 1, DX11::RenderSRV.GetAddressOf());
			RenderPass(PostProcessPass::PP_LUMINANCE);

			DX11::Context->PSSetShaderResources(0, 1, &nullsrv);
			DX11::Context->VSSetShaderResources(0, 1, &nullsrv);
			myHalfSize->Clear();
			myHalfSize->SetAsTarget();
			myFullSize->SetAsResource(0);
			RenderPass(PostProcessPass::PP_COPY);

			DX11::Context->PSSetShaderResources(0, 1, &nullsrv);
			DX11::Context->VSSetShaderResources(0, 1, &nullsrv);
			myQuarterSize->Clear();
			myQuarterSize->SetAsTarget();
			myHalfSize->SetAsResource(0);
			RenderPass(PostProcessPass::PP_COPY);

			DX11::Context->PSSetShaderResources(0, 1, &nullsrv);
			DX11::Context->VSSetShaderResources(0, 1, &nullsrv);
			myBlur->Clear();
			myBlur->SetAsTarget();
			myQuarterSize->SetAsResource(0);
			RenderPass(PostProcessPass::PP_GAUSSIAN);

			DX11::Context->PSSetShaderResources(0, 1, &nullsrv);
			DX11::Context->VSSetShaderResources(0, 1, &nullsrv);
			myHalfSize->Clear();
			myHalfSize->SetAsTarget();
			myBlur->SetAsResource(0);
			RenderPass(PostProcessPass::PP_COPY);

			DX11::Context->PSSetShaderResources(0, 1, &nullsrv);
			DX11::Context->VSSetShaderResources(0, 1, &nullsrv);
			myFullSize->Clear();
			myFullSize->SetAsTarget();
			DX11::Context->OMSetRenderTargets(1, &buffers[0], DX11::DepthBuffer.Get());
			myBlur->SetAsResource(0);
			RenderPass(PostProcessPass::PP_COPY);

			DX11::Context->PSSetShaderResources(0, 1, &nullsrv);
			DX11::Context->VSSetShaderResources(0, 1, &nullsrv);
			DX11::Context->OMSetRenderTargets(1, &buffers[0], DX11::DepthBuffer.Get());
			myFullSize->SetAsResource(0);
			myBlur->SetAsResource(1);
			RenderPass(PostProcessPass::PP_BLOOM);


			break;
		}

		case PP_SSAO:
		{
			myNoiseTexture->SetAsResource(9);

			HRESULT result = S_FALSE;
			D3D11_MAPPED_SUBRESOURCE bufferData;
			RECT clientRect = DX11::GetClientSize();
			const Vector2ui Resolution = {
				static_cast<unsigned int>(clientRect.right - clientRect.left),
				static_cast<unsigned int>(clientRect.bottom - clientRect.top)
			};

			myFrameBufferData.View = Matrix4x4f::GetFastInverse(Renderer::GetViewMatrix());
			myFrameBufferData.CamTranslation = Renderer::GetViewMatrix().GetPosition();
			myFrameBufferData.Projection = Renderer::GetProjectionMatrix();
			myFrameBufferData.RenderMode = 0;

			myFrameBufferData.Resolution = Resolution;
			myFrameBufferData.FarPlane = Renderer::GetCamera()->myFarPlane;
			myFrameBufferData.NearPlane = Renderer::GetCamera()->myNearPlane;

			myFrameBufferData.DeltaTime = 0;
			myFrameBufferData.TotalTime = 0;
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

			SetDepthStencilState(DepthStencilState::ReadWrite);
			SetBlendState(BlendState::Additive);
			mySSAOTexture->Clear();
			DX11::Context->PSSetShaderResources(8, 1, &nullsrv);
			DX11::Context->VSSetShaderResources(8, 1, &nullsrv);

			mySSAOTexture->SetAsTarget();
			RenderPass(PostProcessPass::PP_SSAO);

			DX11::Context->OMSetRenderTargets(1, &buffers[0], DX11::DepthBuffer.Get());
			mySSAOTexture->SetAsResource(8);

			break;
		}

		case PP_TONEMAP:
		{
			ID3D11ShaderResourceView* nullsrv = nullptr;

			SetDepthStencilState(DepthStencilState::Disabled);
			SetBlendState(BlendState::Additive);
			DX11::Context->PSSetShaderResources(0, 1, &nullsrv);
			DX11::Context->VSSetShaderResources(0, 1, &nullsrv);
			myFullSize->Clear();
			myFullSize->SetAsTarget();
			DX11::Context->PSSetShaderResources(0, 1, DX11::RenderSRV.GetAddressOf());
			DX11::Context->VSSetShaderResources(0, 1, DX11::RenderSRV.GetAddressOf());
			RenderPass(PostProcessPass::PP_TONEMAP);

			DX11::Context->PSSetShaderResources(0, 1, &nullsrv);
			DX11::Context->VSSetShaderResources(0, 1, &nullsrv);
			
			DX11::Context->OMSetRenderTargets(1, &buffers[0], DX11::DepthBuffer.Get());
			myFullSize->SetAsResource(0);
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
	mySSAOTexture->Clear();
}
