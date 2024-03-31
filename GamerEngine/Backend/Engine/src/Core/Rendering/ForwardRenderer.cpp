#include "GraphicsEngine.pch.h"
#include "ForwardRenderer.h"
#include <Core/Framework/DX11.h>
#include "GraphicsEngine.h"
#include "Renderer.h"
#include "Light/DirectionalLight.h"
#include "Light/EnvironmentLight.h"
#include "Sort/Sort.hpp"
#include <Particles/particleemitter.h>

#include "AssetHandlers/ModelAssetHandler.h"
#include "Font/MSDFData.h"

bool ForwardRenderer::Initialize()
{

	HRESULT result = S_FALSE;

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

	bufferDescription.ByteWidth = sizeof(SceneLightBuffer);
	result = DX11::Get().GetDevice()->CreateBuffer(&bufferDescription, nullptr, myLightBuffer.GetAddressOf());
	if(FAILED(result))
	{
		return false;
	}

	bufferDescription.ByteWidth = sizeof(TextVertexBuffer);
	result = DX11::Get().GetDevice()->CreateBuffer(&bufferDescription, nullptr, myTextBuffer.GetAddressOf());
	if (FAILED(result))
	{
		return false;
	}


	BuildDepth();

	return true;
}

void ForwardRenderer::Render(Matrix4x4f aView, Matrix4x4f aProjection, const std::vector<RenderBuffer>& aModelList, const Ref<DirectionalLight>& aDirectionalLight, const Ref<EnvironmentLight>& anEnvironmentLight, const std::vector<Light*>& aLightList, VREye anEye)
{
	if(!Renderer::GetCamera())
	{
		return;
	}


	if(aModelList.empty())
	{
		return;
	}


	HRESULT result = S_FALSE;
	D3D11_MAPPED_SUBRESOURCE bufferData;





	myFrameBufferData.View = Matrix4x4f::GetFastInverse(aView);
	myFrameBufferData.CamTranslation = aView.GetPosition();
	myFrameBufferData.Projection = aProjection;
	myFrameBufferData.RenderMode = static_cast<int>(GraphicsEngine::Get()->GetRenderMode());
	myFrameBufferData.FarPlane = Renderer::GetCamera()->myFarPlane;
	myFrameBufferData.NearPlane = Renderer::GetCamera()->myNearPlane;


	Rect clientRect = DX11::Get().GetClientSize();
	uint32_t width = anEye == VREye::None ? clientRect.Right - clientRect.Left : DX11::Get().GetScreenSize().x;
	uint32_t height = anEye == VREye::None ? clientRect.Bottom - clientRect.Top : DX11::Get().GetScreenSize().y;

	const Vector2ui Resolution = {
		width,
		height
	};
	myFrameBufferData.Resolution = Resolution;

	ZeroMemory(&bufferData, sizeof(D3D11_MAPPED_SUBRESOURCE));
	result = DX11::Get().GetContext()->Map(myFrameBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferData);
	if (FAILED(result))
	{
		// BOOM?
		return;
	}

	memcpy(bufferData.pData, &myFrameBufferData, sizeof(FrameBufferData));
	DX11::Get().GetContext()->Unmap(myFrameBuffer.Get(), 0);

	DX11::Get().GetContext()->VSSetConstantBuffers(0, 1, myFrameBuffer.GetAddressOf());
	DX11::Get().GetContext()->PSSetConstantBuffers(0, 1, myFrameBuffer.GetAddressOf());


	if (aDirectionalLight)
	{
		mySceneLightBufferData.DirectionalLight = aDirectionalLight->GetLightBufferData();
		aDirectionalLight->SetAsResource(nullptr, 0);
	}

	if (anEnvironmentLight)
	{
		anEnvironmentLight->SetAsResource(nullptr, 0);
	}

	mySceneLightBufferData.NumLightsPoint = 0;
	mySceneLightBufferData.NumLightsSpot = 0;
	ZeroMemory(mySceneLightBufferData.LightsPoint, sizeof(Light::LightBufferData) * MAX_FORWARD_LIGHTS);
	ZeroMemory(mySceneLightBufferData.LightsSpot, sizeof(Light::LightBufferData) * MAX_FORWARD_LIGHTS);

	for (size_t l = 0; l < aLightList.size(); l++)
	{
		if (aLightList[l]->GetLightBufferData().LightType == 1)
		{
			continue;
		}
		else
		{
			if (aLightList[l]->GetLightBufferData().LightType == 2 && mySceneLightBufferData.NumLightsPoint < MAX_FORWARD_LIGHTS)
			{
				mySceneLightBufferData.LightsPoint[mySceneLightBufferData.NumLightsPoint] = aLightList[l]->GetLightBufferData();
				aLightList[l]->SetAsResource(nullptr, mySceneLightBufferData.NumLightsPoint);
				mySceneLightBufferData.NumLightsPoint++;

			}

			if (aLightList[l]->GetLightBufferData().LightType == 3 && mySceneLightBufferData.NumLightsSpot < MAX_FORWARD_LIGHTS)
			{
				mySceneLightBufferData.LightsSpot[mySceneLightBufferData.NumLightsSpot] = aLightList[l]->GetLightBufferData();
				aLightList[l]->SetAsResource(nullptr, mySceneLightBufferData.NumLightsSpot);
				mySceneLightBufferData.NumLightsSpot++;

			}


		}
	}


	result = DX11::Get().GetContext()->Map(
		myLightBuffer.Get(),
		0,
		D3D11_MAP_WRITE_DISCARD,
		0,
		&bufferData);

	if (FAILED(result))
	{
		// NOOOOOOOOOOOOOO :(
	}

	memcpy(bufferData.pData, &mySceneLightBufferData, sizeof(SceneLightBuffer));

	DX11::Get().GetContext()->Unmap(myLightBuffer.Get(), 0);
	DX11::Get().GetContext()->PSSetConstantBuffers(3, 1, myLightBuffer.GetAddressOf());


	for(const RenderBuffer& modelBuffer : aModelList)
	{
		Ref<GamerEngine::Model> model = modelBuffer.myModel;


		if(model == nullptr)
		{
			return;
		}

		if(model->RenderWithDeferred())
		{
			continue;
		}

		

		/*SetBlendState(BlendState::Alpha);
		SetDepthStencilState(DepthStencilState::DSS_ReadWrite);*/

		
		//ModelAssetHandler::Get().ResetRenderedModels();



		Ref<GamerEngine::Model> modelInstance = model;

		//bool isInstanced = modelInstance->HasRenderedInstance();
		bool isInstanced = false;

		myObjectBufferData.IsInstanced = isInstanced;
		myObjectBufferData.World = modelBuffer.myTransform;
		myObjectBufferData.WorldInv = Matrix4x4f::GetFastInverse(modelBuffer.myTransform);
		ZeroMemory(&bufferData, sizeof(D3D11_MAPPED_SUBRESOURCE));

		myObjectBufferData.myHasBones = false;
		if(model->GetSkeleton()->GetRoot())
		{
			auto bones = modelInstance->GetBoneTransform();
			myObjectBufferData.myHasBones = true;
			memcpy_s(
				&myObjectBufferData.BoneData[0], sizeof(Matrix4x4f) * MAX_MODEL_BONES,
				&bones[0], sizeof(Matrix4x4f) * MAX_MODEL_BONES
			);
		}

		result = DX11::Get().GetContext()->Map(myObjectBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferData);
		if(FAILED(result))
		{
			// WTF
		}

		memcpy(bufferData.pData, &myObjectBufferData, sizeof(ObjectBufferData));
		DX11::Get().GetContext()->Unmap(myObjectBuffer.Get(), 0);

		DX11::Get().GetContext()->VSSetConstantBuffers(1, 1, myObjectBuffer.GetAddressOf());
		DX11::Get().GetContext()->PSSetConstantBuffers(1, 1, myObjectBuffer.GetAddressOf());

		for(int index = 0; index < modelInstance->GetNumMeshes(); index++)
		{
			GamerEngine::Model::MeshData& meshData = modelInstance->GetMeshData(index);

			DX11::Get().GetContext()->IASetInputLayout(meshData.myVertexShader->GetInputLayout().Get());
			DX11::Get().GetContext()->VSSetShader(meshData.myVertexShader->Get().Get(), nullptr, 0);
			DX11::Get().GetContext()->PSSetShader(meshData.myPixelShader->Get().Get(), nullptr, 0);
			DX11::Get().GetContext()->IASetIndexBuffer(meshData.myIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
			DX11::Get().GetContext()->IASetPrimitiveTopology(static_cast<D3D_PRIMITIVE_TOPOLOGY>(meshData.myPrimitiveTopology)); // Use D3D11_PRIMITIVE_TOPOLOGY_... for your specific primitive type

			
			meshData.MaterialData.SetAsResource(myMaterialBuffer.Get());
			DX11::Get().GetContext()->PSSetConstantBuffers(2, 1, myMaterialBuffer.GetAddressOf());
			
			//DX11::Get().GetContext()->PSSetConstantBuffers(2, 1, myMaterialBuffer.GetAddressOf());

			/*DX11::Get().GetContext()->VSSetConstantBuffers(2, 1, myMaterialBuffer.GetAddressOf());
			DX11::Get().GetContext()->PSSetConstantBuffers(2, 1, myMaterialBuffer.GetAddressOf());
*/

//SetSamplerState(SamplerState::SS_Wrap);

			/*if(isInstanced && !model->HasBeenRendered())
			{
				myInstancedTransformBufferData.clear();
				std::vector<Model::RenderedInstanceData> myTransformData = model->GetTransformVector();
				for(int i = 0; i < myTransformData.size(); i++)
				{
					auto matrix = myTransformData[i].World->GetMatrix();
					myInstancedTransformBufferData.push_back(matrix);
				}

				ZeroMemory(&bufferData, sizeof(D3D11_MAPPED_SUBRESOURCE));
				result = DX11::Get().GetContext()->Map(myInstanceBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferData);
				memcpy(bufferData.pData, &myInstancedTransformBufferData[0], sizeof(Matrix4x4f) * model->GetNumberOfInstances());
				DX11::Get().GetContext()->Unmap(myInstanceBuffer.Get(), 0);

				ID3D11Buffer* buffers[2] = { meshData.myVertexBuffer.Get(), myInstanceBuffer.Get() };

				UINT stride[2] = { meshData.myStride, sizeof(Matrix4x4f) };

				UINT offset[2] = { meshData.myOffset, 0 };

				DX11::Get().GetContext()->IASetVertexBuffers(0, 2, buffers, stride, offset);
				DX11::Get().GetContext()->DrawIndexedInstanced(
					meshData.myNumberOfIndices,
					model->GetNumberOfInstances(),
					0, 0, 0
				);
			}
			else
			{*/
				DX11::Get().GetContext()->IASetVertexBuffers(0, 1, meshData.myVertexBuffer.GetAddressOf(), &meshData.myStride, &meshData.myOffset);
				DX11::Get().GetContext()->DrawIndexed(meshData.myNumberOfIndices, 0, 0);
			//}
		}

		//if(isInstanced)
		//{
		//	modelInstance->SetHasBeenRenderer(true);
		//}

	}

}

void ForwardRenderer::RenderSprites(Matrix4x4f aView, Matrix4x4f aProjection, std::vector<RenderBuffer2D>& aSpriteList,
	const Ref<DirectionalLight>& aDirectionalLight,
	const Ref<EnvironmentLight>& anEnvironmentLight)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE bufferData;

	myFrameBufferData.View = Matrix4x4f::GetFastInverse(aView);
	myFrameBufferData.Projection = aProjection;

	ZeroMemory(&bufferData, sizeof(D3D11_MAPPED_SUBRESOURCE));
	result = DX11::Get().GetContext()->Map(myFrameBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferData);
	if(FAILED(result))
	{
		// it ded
	}
	memcpy(bufferData.pData, &myFrameBufferData, sizeof(FrameBufferData));

	DX11::Get().GetContext()->Unmap(myFrameBuffer.Get(), 0);
	//SetDepthStencilState(DepthStencilState::DSS_ReadOnly);


	DX11::Get().GetContext()->VSSetConstantBuffers(0, 1, myFrameBuffer.GetAddressOf());
	DX11::Get().GetContext()->GSSetConstantBuffers(0, 1, myFrameBuffer.GetAddressOf());
	DX11::Get().GetContext()->PSSetConstantBuffers(0, 1, myFrameBuffer.GetAddressOf());
	DX11::Get().GetContext()->VSSetConstantBuffers(1, 1, myObjectBuffer.GetAddressOf());
	DX11::Get().GetContext()->PSSetConstantBuffers(1, 1, myObjectBuffer.GetAddressOf());
	CommonUtilities::MergeSort(aSpriteList);


	for(RenderBuffer2D rc : aSpriteList)
	{
		ZeroMemory(&bufferData, sizeof(D3D11_MAPPED_SUBRESOURCE));
		result = DX11::Get().GetContext()->Map(myObjectBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferData);
		if(FAILED(result))
		{
			// it ded
		}
		memcpy(bufferData.pData, &myObjectBufferData, sizeof(ObjectBufferData));
		DX11::Get().GetContext()->Unmap(myObjectBuffer.Get(), 0);

		//SetDepthStencilState(DepthStencilState::DSS_ReadOnly);

		//SetSamplerState(SamplerState::SS_Wrap);

		if(rc.mySprite)
		{
			rc.mySprite->SetAsResource();
			rc.mySprite->Draw();
		}

	}
}

void ForwardRenderer::RenderString(const std::string& aString, Ref<Engine::Font> aFont, Matrix4x4f aTransform,
	Vector4f aColor)
{
	const auto& fontGeometry = aFont->GetMSDFData()->FontGeometry;
	const auto& metrics = fontGeometry.getMetrics();
	auto fontAtlas = aFont->GetAtlasTexture();

	double x = 0.0;
	double fsScale = 1.0 / (metrics.ascenderY - metrics.descenderY);
	double y = 0.0;

	char character = 'C';
	auto glyph = fontGeometry.getGlyph(character);

	if (!glyph)
	{
		glyph = fontGeometry.getGlyph('?');
	}

	if (!glyph)
	{
		return;
	}

	double al, ab, ar, at;
	glyph->getQuadAtlasBounds(al, ab, ar, at);

	Vector2f texCoordMin((float)al, (float)ab);
	Vector2f texCoordMax((float)ar, (float)at);


	double pl, pb, pr, pt;
	glyph->getQuadPlaneBounds(pl, pb, pr, pt);
	Vector2f quadMin((float)al, (float)ab);
	Vector2f quadMax((float)ar, (float)at);

	quadMin *= (float)fsScale, quadMin *= (float)fsScale;
	quadMin += Vector2f{ (float)x,(float)y };
	quadMax += Vector2f{ (float)x,(float)y };

	float texelWidth = 1.0f / (float)fontAtlas->GetWidth();
	float texelHeight = 1.0f / (float)fontAtlas->GetHeight();
	texCoordMin += Vector2f(texelWidth, texelHeight);
	texCoordMax += Vector2f(texelWidth, texelHeight);

	// Render

	double advance = glyph->getAdvance();
	char nextCharacter = 'C';
	fontGeometry.getAdvance(advance, character, nextCharacter);

	float kerningOffset = 0.0f;
	x += fsScale * advance + kerningOffset;

}

void ForwardRenderer::BuildDepth()
{}

void ForwardRenderer::ClearBuffer()
{}

void ForwardRenderer::Release()
{
	myFrameBuffer->Release();
	myObjectBuffer->Release();
	myLightBuffer->Release();
	//myMaterialBuffer->Release();

	/*for (int i = 0; i < myDepthStencilStates.size(); i++)
	{
		myDepthStencilStates[i] = nullptr;
	}*/
}
