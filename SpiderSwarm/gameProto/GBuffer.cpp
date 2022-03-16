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
	// フレームバッファオブジェクトを作成してmBufferIDに保存

	// フレームバッファ用の深度バッファを追加

	// Gバッファの出力用にテクスチャを作成
	for (int i = 0; i < GBUFFER_TEXTURE_NUM; i++)
	{
		Texture* t = new Texture();
		t->CreateForRendering(width,height,GL_RGB32F);
		mTextures.emplace_back(t);

		// このテクスチャをカラー出力にアタッチ
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, t->GetTextureID(),0);
	}

	// カラーアタッチメントの配列を作成
	std::vector<GLenum> attachments;
	for (int i = 0; i < GBUFFER_TEXTURE_NUM; i++)
	{
		attachments.emplace_back(GL_COLOR_ATTACHMENT0 + i);
	}

	// 描画先バッファのリストを設定
	glDrawBuffers(static_cast<GLsizei>(attachments.size()), attachments.data());

	// フレームバッファが正しく作成されてない場合破棄
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
