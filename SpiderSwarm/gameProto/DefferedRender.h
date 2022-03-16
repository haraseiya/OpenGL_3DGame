#pragma once

#include <vector>

class Shader;

class DefferedRender
{
public:
	DefferedRender();
	~DefferedRender();

	void Begin();
	void End();

private:
	Shader* m_shader;

	unsigned int m_gBuffer;		// GBuffer
	unsigned int m_gPosition;	// �ʒu�p�J���[�o�b�t�@
	unsigned int m_gNormal;		// �@���p�J���[�o�b�t�@
	unsigned int m_gAlbedoSpec;	// �A���x�h�iRGB�j&�X�y�L�����[�iA�j�p�J���[�o�b�t�@
	unsigned int m_gRBO;		// �����_�[�o�b�t�@

	unsigned int m_attachments[3];

	// �o�b�t�@�[�e�N�X�`��
	std::vector<unsigned int>m_gBufferTexture;
};

