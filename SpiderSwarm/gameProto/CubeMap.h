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

	unsigned int mCubeMapVAO;			// �L���[�u�}�b�v���_�z��
	unsigned int mCubeMapVBO;			// �L���[�u�}�b�v

	// �X�J�C�{�b�N�X�p
	unsigned int mSkyBoxVAO;		// �L���[�u�}�b�v���_�z��
	unsigned int mSkyBoxVBO;

	int mWidth;		// ��
	int mHeight;		// ����
	int mChannel;	// �`�����l��
};

