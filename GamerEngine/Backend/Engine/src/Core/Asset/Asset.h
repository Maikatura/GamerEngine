#pragma once
#include "Scene/UUID.h"


namespace GamerEngine
{
	using AssetHandle = UUID;


	enum class AssetType
	{
		None = 0,
		Scene,
		Texture2D,
		Mesh,
		Texture,
		Audio,
		TextureCube,


		Count
	};

	class Asset
	{
	public:
		AssetHandle Handle = 0;

		virtual  AssetType GetType() const = 0;
	};
}
