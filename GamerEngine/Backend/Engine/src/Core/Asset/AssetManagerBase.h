#pragma once
#include "Asset.h"


namespace GamerEngine
{
	class AssetManagerBase
	{
	public:
		virtual Ref<Asset> GetAsset(AssetHandle aHandle) const = 0;
 	};
}
