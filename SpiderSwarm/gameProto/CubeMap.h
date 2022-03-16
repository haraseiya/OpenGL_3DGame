#pragma once

#include <vector>
#include <string>

class Shader;
class Texture;
class CameraActor;

class CubeMap
{
public:
	CubeMap();
	~CubeMap();

	unsigned int LoadTexture(char const* path);
	unsigned int LoadCubeMap(std::vector<std::string>_faces);
	void Draw();

private:
	Texture* mTexture;
	CameraActor* mCamera;

	Shader* mCubeMapShader;
	Shader* mSkyBoxShader;	

	std::vector<std::string> mFaces;

	unsigned int mCubeMapTexture;
	unsigned char* mData;

	unsigned int mCubeMapVAO;			// キューブマップ頂点配列
	unsigned int mCubeMapVBO;			// キューブマップ

	// スカイボックス用
	unsigned int mSkyBoxVAO;		// キューブマップ頂点配列
	unsigned int mSkyBoxVBO;

	int mWidth;		// 幅
	int mHeight;		// 高さ
	int mChannel;	// チャンネル
};

