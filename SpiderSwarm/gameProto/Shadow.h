#pragma once

#include <GL/glew.h>
#include <string>

class Shadow
{
public:
	Shadow(const std::string& _vert, const std::string& _frag);
	~Shadow();

	// ��ʑS�̂𕢂����_��`
	void screenVAOSetting(unsigned int& vao);

	// �e�̍X�V
	void Update();

private:
	// �V�F�[�_�[
	class Shader* m_shader;
	
	// �e�N�X�`���N���X
	//class Texture* m_texture;

	// �e�̉𑜓x
	static const int SHADOW_WIDTH;
	static const int SHADOW_HEIGHT;

	// �t���[���o�b�t�@�p
	unsigned int m_depthMapFBO;

	// �X�N���[���S�̂𕢂����_�o�b�t�@�I�u�W�F�N�g
	unsigned int m_vao;

	// �e�N�X�`���p
	unsigned int m_texture;

	// ���C�g��ԗp�s��
	Matrix4 m_lightProjection;
	Matrix4 m_lightView;
	Matrix4 m_lightSpace;
};