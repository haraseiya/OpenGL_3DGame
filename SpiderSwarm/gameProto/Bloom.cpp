#include "Bloom.h"
#include "Shader.h"
#include <gl/glew.h>
#include <iostream>

Bloom::Bloom()
{
    // �V�F�[�_�[�̓ǂݍ���
    m_shader = new Shader();
    m_shader->Load("Bloom.vert", "Bloom.frag");

    glGenFramebuffers(1, &m_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

    glGenTextures(2, m_colorBuffers);
    for (unsigned int i = 0; i < 2; i++)
    {
        glBindTexture(GL_TEXTURE_2D, m_colorBuffers[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, 1024, 768, 0, GL_RGBA, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        // �t���[���o�b�t�@�Ƀe�N�X�`�����A�^�b�`
        glFramebufferTexture2D(
            GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, m_colorBuffers[i], 0
        );
    }

    // �����_�[�o�b�t�@�[�̍쐬
    glGenRenderbuffers(1, &m_rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, m_rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 1024, 768);

    // FBO�Ƀ����_�[�o�b�t�@�[���A�^�b�`����
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_rbo);
    m_attachments[0] = GL_COLOR_ATTACHMENT0;
    m_attachments[1] = GL_COLOR_ATTACHMENT1;
    glDrawBuffers(2, m_attachments);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0); // �f�t�H���g�ɖ߂�
}

Bloom::~Bloom()
{
}

void Bloom::Update()
{
    //// �`��Ώۂ��t���[���o�b�t�@�ɂ���(�`��Ώۂ͂Q������)
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

        glEnable(GL_DEPTH_TEST);

        // �`��p�X
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // �V�F�[�_�[��uniform�ϐ����Z�b�g
        m_shader->SetActive();
        m_shader->SetVectorUniform("light.direction", LightDir);
        m_shader->SetVectorUniform("light.ambient", ambient);
        m_shader->SetVectorUniform("light.diffuse", diffuse);
        m_shader->SetVectorUniform("light.specular", specular);
        m_shader->SetVectorUniform("viewPos", viewPos);
        m_shader->SetMatrixUniform("view", viewMat.GetAsFloatPtr());
        m_shader->SetMatrixUniform("projection", projMat.GetAsFloatPtr());
}
