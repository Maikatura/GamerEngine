#pragma once


#include "fbxsdk.h"
#include "../TgaFbxStructs.h"

namespace GamerEngine
{
	class FbxContext
	{
	public:
		FbxContext(const char* aFile)
		{
			
		}

	private:
		FbxManager* myFBXManager{ nullptr };
		FbxScene* myFBXScene{ nullptr };
		float mySceneScale{ 1.0f };
	};
}