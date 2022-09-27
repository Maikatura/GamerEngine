#pragma once
#include <Math/Matrix4x4.hpp>
#include <wrl.h>

#include <vector>
#include <Model/Model.h>
#include <Model/ModelInstance.h>

#include "RenderBuffer.h"

class ForwardRenderer
{
	struct FrameBufferData 
	{
		CommonUtilities::Matrix4x4<float> View;
		CommonUtilities::Matrix4x4<float> Projection;
	} myFrameBufferData;

	struct ObjectBufferData
	{
		CommonUtilities::Matrix4x4<float> World;
		CommonUtilities::Matrix4x4<float> myBoneData[128]{};
		bool myHasBones;
		CommonUtilities::Vector3<float> myPadding;
	} myObjectBufferData {};

	Microsoft::WRL::ComPtr<ID3D11Buffer> myFrameBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> myObjectBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> myMaterialBuffer;

public:
	bool Initialize();

	void Render(const std::vector<RenderBuffer>& aModelList);

	void Release();
};

