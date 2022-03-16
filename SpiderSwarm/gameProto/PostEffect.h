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
	Shader* m_shader;	// シェーダー

	unsigned int m_fbo;	// フレームバッファ
	unsigned int m_rbo;	// レンダーバッファ

	unsigned int m_quadVAO;
	unsigned int m_quadVBO;

	GLuint m_texture;		// テクスチャ

};