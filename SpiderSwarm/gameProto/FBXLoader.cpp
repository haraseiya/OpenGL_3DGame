#include "FBXLoader.h"

FBXLoader::FBXLoader()
	: 
{
	m_manager = FbxManager::Create();
	m_importer = FbxImporter::Create(m_manager, "");
}

FBXLoader::~FBXLoader()
{
}
