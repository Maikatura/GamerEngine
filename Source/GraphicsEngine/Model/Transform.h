#pragma once
#include <Math/Matrix4x4.hpp>

class Transform
{
private:
	CommonUtilities::Matrix4x4<float> myTransform;
	
public:

	void SetPosition(CommonUtilities::Vector4<float> aPosition) { myTransform.SetPosition(aPosition); };

	CommonUtilities::Vector4<float> GetPosition() const { return myTransform.GetPosition(); };
	CommonUtilities::Matrix4x4<float> GetMatrix() { return myTransform; };
};

