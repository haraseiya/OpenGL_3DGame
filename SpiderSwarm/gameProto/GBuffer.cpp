#include "GBuffer.h"
#include "Texture.h"
#include "Renderer.h"

GBuffer::GBuffer()
	: mBufferID(0)
{
}

GBuffer::~GBuffer()
{
	mTextures.clear();
}

bool GBuffer::Create(int width, int height)
{
	// �t���[���o�b�t�@�I�u�W�F�N�g���쐬����mBufferID�ɕۑ�

	// �t���[���o�b�t�@�p�̐[�x�o�b�t�@��ǉ�

	// G�o�b�t�@�̏o�͗p�Ƀe�N�X�`�����쐬
	for (int i = 0; i < GBUFFER_TEXTURE_NUM; i++)
	{
		Texture* t = new Texture();
		t->CreateForRendering(width,height,GL_RGB32F);
		mTextures.emplace_back(t);

		// ���̃e�N�X�`�����J���[�o�͂ɃA�^�b�`
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, t->GetTextureID(),0);
	}

	// �J���[�A�^�b�`�����g�̔z����쐬
	std::vector<GLenum> attachments;
	for (int i = 0; i < GBUFFER_TEXTURE_NUM; i++)
	{
		attachments.emplace_back(GL_COLOR_ATTACHMENT0 + i);
	}

	// �`���o�b�t�@�̃��X�g��ݒ�
	glDrawBuffers(static_cast<GLsizei>(attachments.size()), attachments.data());

	// �t���[���o�b�t�@���������쐬����ĂȂ��ꍇ�j��
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		Destroy();
		return false;
	}
	return true;
}

void GBuffer::Destroy()
{
	glDeleteFramebuffers(1, &mBufferID);
	for (Texture* t : mTextures)
	{
		t->Unload();
		delete t;
	}
}

Texture* GBuffer::GetTexture(Type type)
{
	if (mTextures.size() > 0)
	{
		return mTextures[type];
	}
	else
	{
		return nullptr;
	}
}

void GBuffer::SetTextureActive()
{
	for (int i = 0; i < GBUFFER_TEXTURE_NUM; i++)
	{
		mTextures[i]->SetActive(i);
	}
}
