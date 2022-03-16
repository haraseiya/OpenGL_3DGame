#include "Shadow.h"
#include "Shader.h"

const int Shadow::SHADOW_WIDTH = 2048;
const int Shadow::SHADOW_HEIGHT = 2048;

Shadow::Shadow(const std::string& _vert, const std::string& _frag)
{
	// �V�F�[�_�[�ǂݍ���
	m_shader = new Shader();
	if(!m_shader->Load(_vert, _frag)) return;

	// �f�v�X�}�b�v�p�̃t���[���o�b�t�@�𐶐�
	glGenFramebuffers(1, &m_depthMapFBO);

	// �f�v�X�o�b�t�@�Ƃ��Ďg�p����e�N�X�`������
	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);

	// �[�x�l���~�����̂�GL_DEPTH_COMPONENT�ɐݒ�
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// depthMapFBO�Ƀf�v�X�e�N�X�`�����A�^�b�`����
	glBindFramebuffer(GL_FRAMEBUFFER, m_texture);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_texture, 0);

	// �K�v�Ȃ̂͐[�x��񂾂������A�J���[�o�b�t�@�͕K�v�Ȃ�����
	// �����I�ɓǂݍ��݂ƕ`��̗������g�p���Ȃ����Ƃ�`����
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

Shadow::~Shadow()
{
}

void Shadow::screenVAOSetting(unsigned int& _vao)
{
	unsigned int vbo;
	float quadVertices[] = {
		// positions        // texture Coords
		-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
		 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	};
	// VAO�̃Z�b�g
	glGenVertexArrays(1, &_vao);
	glGenBuffers(1, &vbo);
	glBindVertexArray(_vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
}

void Shadow::Update()
{
	m_lightProjection = Matrix4::CreateOrtho(80.0f, 80.0f, 1.0f, 100.0f);
	m_lightView = Matrix4::CreateLookAt(Vector3(0.0f,30.0f,0.0f), Vector3(24,0,-24), Vector3::UnitY);
	m_lightSpace = m_lightView * m_lightProjection;

	// �V���h�E�}�b�v�p�X
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, m_depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	m_shader->SetActive();
	m_shader->SetMatrixUniform("lightSpaceMatrix", m_lightSpace.GetAsFloatPtr());

	glEnable(GL_DEPTH_TEST);
	m_shader->SetActive();
	m_shader->SetIntUniform("depthMap", 2);
	m_shader->SetMatrixUniform("lightSpaceMatrix", m_lightSpace.GetAsFloatPtr());
}
