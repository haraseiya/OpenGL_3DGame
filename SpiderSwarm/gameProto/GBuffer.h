#pragma once
#include <vector>

class Texture;

class GBuffer
{
public:
	// G�o�b�t�@�Ɋi�[�����f�[�^�̎��
	enum Type
	{
		DIFFUSE=0,
		NORMAL,
		WORLD_POS,
		GBUFFER_TEXTURE_NUM
	};

	GBuffer();
	~GBuffer();

	// G�o�b�t�@�̐����Ɣj��
	bool Create(int width,int height);
	void Destroy();

	// �w�肷��e�N�X�`�����擾
	Texture* GetTexture(Type type);

	// �t���[���o�b�t�@�I�u�W�F�N�g��ID���擾
	unsigned int GetBufferID() const { return mBufferID; }

	// �S�Ă�G�o�b�t�@�e�N�X�`����ǂݍ��ݗp�ɐݒ�
	void SetTextureActive();

private:
	std::vector<Texture*> mTextures;	// G�o�b�t�@�Ɋ��蓖�Ă�ꂽ�e�N�X�`��
	unsigned int mBufferID;				// �t���[���o�b�t�@�I�u�W�F�N�gID
};

