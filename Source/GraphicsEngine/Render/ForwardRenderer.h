#pragma once
#include <Math/Matrix4x4.hpp>
#include <wrl.h>
#include <Scene/Camera.h>
#include <vector>
#include <Model/Model.h>

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
	} myObjectBufferData;

	Microsoft::WRL::ComPtr<ID3D11Buffer> myFrameBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> myObjectBuffer;

public:
	bool Initialize();

	void Render(const std::shared_ptr<Camera>& aCamera, const std::vector<std::shared_ptr<Model>>& aModelList);
};

