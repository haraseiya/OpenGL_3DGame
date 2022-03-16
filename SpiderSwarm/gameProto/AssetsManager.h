#pragma once

#include <unordered_map>

class MeshComponent;
class EffectComponent;

class AssetsManager
{
public:
	static AssetsManager& GetInstance()
	{
		static AssetsManager instance;
		return instance;
	}

	~AssetsManager();


private:
	AssetsManager();

	std::unordered_map<std::string, MeshComponent*> mMeshMap;
	std::unordered_map<std::string, EffectComponent*> mEffectMap;
};

#define ASSETS_MANAGER AssetsManager::GetInstance();
