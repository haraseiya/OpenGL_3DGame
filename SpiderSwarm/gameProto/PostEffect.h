#pragma once

#include <GL/glew.h>

class Shader;

class PostEffect
{
public:
	PostEffect();
	~PostEffect();

	void PreUpdate();
	void DifUpdate();
	
private:
	Shader* m_shader;	// �V�F�[�_�[

	unsigned int m_fbo;	// �t���[���o�b�t�@
	unsigned int m_rbo;	// �����_�[�o�b�t�@

	unsigned int m_quadVAO;
	unsigned int m_quadVBO;

	GLuint m_texture;		// �e�N�X�`��

};