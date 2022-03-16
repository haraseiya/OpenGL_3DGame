#pragma once

class Shader;

class Bloom
{
public:
	Bloom();
	~Bloom();

	void Update();

private:
	unsigned int m_fbo;		// �t���[���o�b�t�@�I�u�W�F�N�g
	unsigned int m_rbo;		// �����_�[�o�b�t�@�I�u�W�F�N�g

	unsigned int m_colorBuffers[2];
	unsigned int m_attachments[2];

	Shader* m_shader;
};