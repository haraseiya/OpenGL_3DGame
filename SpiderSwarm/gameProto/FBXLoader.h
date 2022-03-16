#pragma once

#include <fbxsdk.h>

class FBXLoader
{
public:
	FBXLoader();
	~FBXLoader();

private:
	FbxManager* m_manager;
	FbxImporter* m_importer;

	const char m_fileName;
};

