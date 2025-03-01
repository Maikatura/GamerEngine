#include "GraphicsEngine.pch.h"
#include "Core/Rendering/ShadowRenderer.h"

#include <fstream>

#include "Buffers.h"
#include "GraphicsEngine.h"
#include "Renderer.h"
#include "Core/Framework/DX11.h"
#include "AssetHandlers/ModelAssetHandler.h"
#include "Utilites/VisualProfiler.h"

bool ShadowRenderer::OnAdd()
{
	PROFILE_SCOPE("ShadowRenderer::OnAdd");

	if (!Initialize())
	{
		return false;
	}

	return true;
}

void ShadowRenderer::OnRelease()
{

}

void ShadowRenderer::OnUpdate()
{
}

void ShadowRenderer::OnRenderSetup()
{
	RendererBase::SetDepthStencilState(DepthStencilState::Disabled);
	RendererBase::SetBlendState(BlendState::None);
	
	
	
	 {
	 	//PROFILE_CPU_SCOPE("Render Shadows");
	 	ClearResources();
	
	 	
	
	 }
}

void ShadowRenderer::OnRender()
{
	auto scene = SceneManager::Get().GetScene();
	const std::vector<Light*> someLightList = scene->GetLights();
	const std::vector< GamerEngine::RenderBuffer>& modelList = GamerEngine::Renderer::GetModels();

	for (auto& light : someLightList)
	{
		Render(light, modelList);
	}
}

void ShadowRenderer::OnEnd()
{
}

bool ShadowRenderer::Initialize()
{
	D3D11_BUFFER_DESC bufferDescription;
	ZeroMemory(&bufferDescription, sizeof(D3D11_BUFFER_DESC));
	
	bufferDescription.Usage = D3D11_USAGE_DYNAMIC;
	bufferDescription.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	bufferDescription.ByteWidth = sizeof(FrameBufferData);
	HRESULT result = DX11::Get().GetDevice()->CreateBuffer(&bufferDescription, nullptr, myFrameBuffer.GetAddressOf());
	if(FAILED(result))
	{
		return false;
	}

	bufferDescription.ByteWidth = sizeof(ObjectBufferData);
	result = DX11::Get().GetDevice()->CreateBuffer(&bufferDescription, nullptr, myObjectBuffer.GetAddressOf());
	if(FAILED(result))
	{
		return false;
	}

	bufferDescription.ByteWidth = sizeof(Vector4f);
	result = DX11::Get().GetDevice()->CreateBuffer(&bufferDescription, nullptr, myMaterialBuffer.GetAddressOf());
	if(FAILED(result))
	{
		return false;
	}

	bufferDescription.ByteWidth = sizeof(ShadowCubeBuffer);
	result = DX11::Get().GetDevice()->CreateBuffer(&bufferDescription, nullptr, myPointLightBuffer.GetAddressOf());
	if(FAILED(result))
	{
		return false;
	}

	D3D11_BUFFER_DESC bufferDescriptionInstance = { 0 };
	D3D11_SUBRESOURCE_DATA vxSubresource{};
	bufferDescriptionInstance.Usage = D3D11_USAGE_DYNAMIC;
	bufferDescriptionInstance.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDescriptionInstance.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	myInstancedTransformBufferData.resize(128);
	vxSubresource.pSysMem = &myInstancedTransformBufferData[0];
	bufferDescriptionInstance.ByteWidth = sizeof(Matrix4x4f) * 128;
	bufferDescriptionInstance.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	result = DX11::Get().GetDevice()->CreateBuffer(&bufferDescriptionInstance, &vxSubresource, myInstanceBuffer.GetAddressOf());
	if (FAILED(result))
	{
		return false;
	}

	std::ifstream gsFile;
	gsFile.open("Shaders\\ShadowCube_GS.cso", std::ios::binary);
	const std::string gsData = { std::istreambuf_iterator(gsFile), std::istreambuf_iterator<char>() };
	DX11::Get().GetDevice()->CreateGeometryShader(gsData.data(), gsData.size(), nullptr, myShadowGeometryShader.GetAddressOf());
	gsFile.close();

	myDeferredVS = TextureAssetHandler::GetVertexShader("Shaders\\Fullscreen_VS.cso");
	myDeferredPS = TextureAssetHandler::GetPixelShader("Shaders\\Deferred_PS.cso");
	return true;
}

void ShadowRenderer::Render(Light* aLight, const std::vector<GamerEngine::RenderBuffer>& aModelList)
{
	if(aModelList.empty())
	{
		return;
	}

	if(!aLight->HasShadowMap())
	{
		return;
	}

	//aLight->ClearShadowMap();
	aLight->SetShadowMapAsDepth();
	
	D3D11_MAPPED_SUBRESOURCE bufferData;


	//aLight->SetAsResource(nullptr);

	const Light::LightBufferData lightData = aLight->GetLightBufferData();
	const bool isCubeMap = lightData.LightType == 2;

	ZeroMemory(&myFrameBufferData, sizeof(FrameBufferData));
	ZeroMemory(&myObjectBufferData, sizeof(ObjectBufferData));

	myFrameBufferData.View = lightData.LightView[0];
	myFrameBufferData.CamTranslation = lightData.Position;
	myFrameBufferData.Projection = lightData.LightProjection;
	myFrameBufferData.NearPlane = lightData.NearPlane;
	myFrameBufferData.FarPlane = lightData.FarPlane;
	myFrameBufferData.RenderMode = static_cast<unsigned int>(0);
	myFrameBufferData.DeltaTime = 0;
	myFrameBufferData.TotalTime = 0;



	ZeroMemory(&bufferData, sizeof(D3D11_MAPPED_SUBRESOURCE));
	HRESULT result = DX11::Get().GetContext()->Map(myFrameBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferData);
	if (FAILED(result))
	{
		return;
	}
	memcpy_s(bufferData.pData, sizeof(FrameBufferData), &myFrameBufferData, sizeof(FrameBufferData));
	DX11::Get().GetContext()->Unmap(myFrameBuffer.Get(), 0);
	DX11::Get().GetContext()->VSSetConstantBuffers(0, 1, myFrameBuffer.GetAddressOf());
	//DX11::Get().GetContext()->PSSetConstantBuffers(0, 1, myFrameBuffer.GetAddressOf());

	if(isCubeMap)
	{
		myShadowCubeData.SC_Views[0] = lightData.LightView[0];
		myShadowCubeData.SC_Views[1] = lightData.LightView[1];
		myShadowCubeData.SC_Views[2] = lightData.LightView[2];
		myShadowCubeData.SC_Views[3] = lightData.LightView[3];
		myShadowCubeData.SC_Views[4] = lightData.LightView[4];
		myShadowCubeData.SC_Views[5] = lightData.LightView[5];

		myShadowCubeData.SC_LightTranslation = lightData.Position;
		myShadowCubeData.SC_Projection = lightData.LightProjection;
		myShadowCubeData.SC_FarPlane = lightData.FarPlane;


		ZeroMemory(&bufferData, sizeof(D3D11_MAPPED_SUBRESOURCE));
		result = DX11::Get().GetContext()->Map(myPointLightBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferData);
		if (FAILED(result))
		{
			return;
		}
		memcpy_s(bufferData.pData, sizeof(ShadowCubeBuffer), &myShadowCubeData, sizeof(ShadowCubeBuffer));
		DX11::Get().GetContext()->Unmap(myPointLightBuffer.Get(), 0);
		DX11::Get().GetContext()->GSSetConstantBuffers(5, 1, myPointLightBuffer.GetAddressOf());
		//DX11::Get().GetContext()->PSSetConstantBuffers(5, 1, myPointLightBuffer.GetAddressOf());
		DX11::Get().GetContext()->GSSetConstantBuffers(0, 1, myFrameBuffer.GetAddressOf());
	}
	else
	{
		
		DX11::Get().GetContext()->GSSetShader(nullptr, nullptr, 0);
	}
	DX11::Get().GetContext()->PSSetShader(nullptr, nullptr, 0);


	//ModelAssetHandler::Get().ResetRenderedModels();

	

	for(const GamerEngine::RenderBuffer& modelBuffer : aModelList)
	{
		Ref<GamerEngine::Model> model = modelBuffer.myModel;

		if (model == nullptr)
		{
			return;
		}


		const bool isInstanced = model->HasRenderedInstance();


		myObjectBufferData.IsInstanced = isInstanced;
		myObjectBufferData.World = modelBuffer.myTransform;
		myObjectBufferData.WorldInv = Matrix4x4f::GetFastInverse(modelBuffer.myTransform);
		ZeroMemory(&bufferData, sizeof(D3D11_MAPPED_SUBRESOURCE));


		myObjectBufferData.myHasBones = false;
		if (model->GetSkeleton()->GetRoot())
		{
			auto bones = model->GetBoneTransform();
			myObjectBufferData.myHasBones = true;
			memcpy_s(
				&myObjectBufferData.BoneData[0], sizeof(Matrix4x4f) * MAX_MODEL_BONES,
				bones.data(), sizeof(Matrix4x4f) * MAX_MODEL_BONES
			);
		}

		result = DX11::Get().GetContext()->Map(myObjectBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferData);
		if (FAILED(result))
		{
			return;
		}

		memcpy_s(bufferData.pData, sizeof(ObjectBufferData), &myObjectBufferData, sizeof(ObjectBufferData));
		DX11::Get().GetContext()->Unmap(myObjectBuffer.Get(), 0);

		DX11::Get().GetContext()->VSSetConstantBuffers(1, 1, myObjectBuffer.GetAddressOf());
		//DX11::Get().GetContext()->PSSetConstantBuffers(1, 1, myObjectBuffer.GetAddressOf());

		if (isCubeMap)
		{
			DX11::Get().GetContext()->GSSetConstantBuffers(0, 1, myObjectBuffer.GetAddressOf());
			DX11::Get().GetContext()->GSSetShader(myShadowGeometryShader.Get(), nullptr, 0);
		}

		for(unsigned int m = 0; m < model->GetNumMeshes(); m++)
		{
			GamerEngine::Model::MeshData& meshData = model->GetMeshData(m);

			
			DX11::Get().GetContext()->IASetInputLayout(meshData.myVertexShader->GetInputLayout().Get());
			DX11::Get().GetContext()->VSSetShader(meshData.myVertexShader->Get().Get(), nullptr, 0);
			DX11::Get().GetContext()->IASetIndexBuffer(meshData.myIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
			DX11::Get().GetContext()->IASetPrimitiveTopology(static_cast<D3D_PRIMITIVE_TOPOLOGY>(meshData.myPrimitiveTopology));

			meshData.MaterialData.SetAsResource(myMaterialBuffer.Get());
			//DX11::Get().GetContext()->PSSetConstantBuffers(2, 1, myMaterialBuffer.GetAddressOf());


			if (isInstanced)
			{
				myInstancedTransformBufferData.clear();
				const std::vector<GamerEngine::Model::RenderedInstanceData>& myTransformData = model->GetTransformVector();
				for (int i = 0; i < myTransformData.size(); i++)
				{
					auto matrix = myTransformData[i].World;
					myInstancedTransformBufferData.push_back(matrix);
				}

				unsigned int instanceCount = static_cast<unsigned int>(myInstancedTransformBufferData.size());

				ZeroMemory(&bufferData, sizeof(D3D11_MAPPED_SUBRESOURCE));
				result = DX11::Get().GetContext()->Map(myInstanceBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferData);
				memcpy(bufferData.pData, myInstancedTransformBufferData.data(), sizeof(Matrix4x4f) * instanceCount);
				DX11::Get().GetContext()->Unmap(myInstanceBuffer.Get(), 0);

				ID3D11Buffer* buffers[2] = { meshData.myVertexBuffer.Get(), myInstanceBuffer.Get() };

				UINT stride[2] = { meshData.myStride, sizeof(Matrix4x4f) };

				UINT offset[2] = { meshData.myOffset, 0 };

				DX11::Get().GetContext()->IASetVertexBuffers(0, 2, buffers, stride, offset);
				DX11::Get().GetContext()->DrawIndexedInstanced(
					meshData.myNumberOfIndices,
					instanceCount,
					0, 0, 0
				);
			}
			else
			{
				DX11::Get().GetContext()->IASetVertexBuffers(0, 1, meshData.myVertexBuffer.GetAddressOf(), &meshData.myStride, &meshData.myOffset);
				DX11::Get().GetContext()->DrawIndexed(meshData.myNumberOfIndices, 0, 0);
			}
		}

		/*if(isInstanced)
		{
			model->SetHasBeenRenderer(true);
		}*/
	}


	DX11::Get().GetContext()->GSSetShader(nullptr, nullptr, 0);
	DX11::Get().ResetViewport();
	//DX11::Get().GetContext()->OMSetRenderTargets(0, nullptr, nullptr);
}

void ShadowRenderer::ClearResources()
{
	ID3D11ShaderResourceView* srvNull = nullptr;
	
	DX11::Get().GetContext()->PSSetShaderResources(19, 1, &srvNull);

	for(int i = 0; i < 40; i++)
	{
		DX11::Get().GetContext()->PSSetShaderResources(20 + i, 1, &srvNull);
	}
}

void ShadowRenderer::ClearTarget()
{

}
