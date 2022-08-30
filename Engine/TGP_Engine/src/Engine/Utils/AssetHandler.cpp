#include "AssetHandler.h"

std::map<std::string, Engine::Ref<Engine::Asset>> Engine::AssetHandler::myRegistry;
void Engine::AssetHandler::Store(const std::string& assetFileName, Ref<Asset> asset)
{
	myRegistry[assetFileName] = asset;
}

bool Engine::AssetHandler::Has(const std::string& assetFileName)
{
	return myRegistry.find(assetFileName) != myRegistry.end();
}

Engine::Ref<Engine::Asset> Engine::AssetHandler::Get(const std::string& assetFileName)
{
	auto asset = myRegistry[assetFileName];
	Asset* assetPtr = asset.get();
	memcpy(assetPtr, myRegistry[assetFileName].get(), sizeof(Asset));
	return std::make_shared<Asset>(*assetPtr);
}
