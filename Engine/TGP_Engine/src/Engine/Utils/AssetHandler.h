#pragma once
#include <map>
#include <string>
#include <string_view>
#include "Engine/Assets/Asset.h"
#include "Engine/Utils/Utilities.h"

namespace Engine
{
	class AssetHandler
	{
	public:
		static void Store(const std::string& assetFileName, Ref<Asset> asset);
		static bool Has(const std::string& assetFileName);
		static Ref<Asset> Get(const std::string& assetFileName);
	private:
		static std::map<std::string, Ref<Asset>> myRegistry;
	};
}
