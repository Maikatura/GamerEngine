#pragma once
#include <Math/Matrix4x4.hpp>
#include <wrl.h>

#include <vector>
#include <Model/Model.h>
#include <Model/ModelInstance.h>

#include "RenderBuffer.h"
#include "Light/DirectionalLight.h"
#include "Light/EnvironmentLight.h"

enum class BlendState
{
	NoBlend,
	AlphaBlend,
	AdditiveBlend,
	BSCount
};

enum class DepthStencilState
{
	DSSReadWrite,
	DSSReadOnly,
	DSSIgnore,
	DSSOverlay,
	DSSCount
};

enum class SamplerState
{
	SSTrilinearClamp,
	SSPointClamp,
	SSCount
};


struct FrameBufferData
{
	Matrix4x4f View;
	Matrix4x4f Projection;
	Vector3f CamTranslation;
	unsigned int RenderMode;
};

struct ObjectBufferData
{
	Matrix4x4f World;
	int myHasBones;
	Vector3f myPadding1;
	int myObjectId = 0;
	Vector3f myPadding2;
	Matrix4x4f myBoneData[128]{};
};

class ForwardRenderer
{
	FrameBufferData myFrameBufferData {};

	ObjectBufferData myObjectBufferData {};

	Microsoft::WRL::ComPtr<ID3D11Buffer> myFrameBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> myObjectBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> myMaterialBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> myLightBuffer;

	std::array<ComPtr<ID3D11DepthStencilState>, (int)DepthStencilState::DSSCount> myDepthStencilStates;
	std::array<ComPtr<ID3D11BlendState>, (int)BlendState::BSCount> myBlendStates;
	std::array<ComPtr<ID3D11SamplerState>, (int)SamplerState::SSCount> mySamplerStates;

public:

	void SetDepthStencilState(DepthStencilState aDepthStencilState);
	void SetBlendState(BlendState aBlendState);




	bool Initialize();

	void Render(const std::vector<RenderBuffer>& aModelList, const std::shared_ptr<DirectionalLight>& aDirectionalLight, const std::
	            shared_ptr<EnvironmentLight>& anEnvironmentLight);

	void RenderSprites(std::vector<RenderBuffer2D>& aSpriteList, const std::shared_ptr<DirectionalLight>& aDirectionalLight, const std::
		shared_ptr<EnvironmentLight>& anEnvironmentLight);

	void BuildDepth();

	void ClearBuffer();

	void Release();
};

