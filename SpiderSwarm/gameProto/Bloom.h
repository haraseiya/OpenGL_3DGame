#pragma once

class Shader;

class Bloom
{
public:
	Bloom();
	~Bloom();

	void Update();

private:
	unsigned int m_fbo;		// フレームバッファオブジェクト
	unsigned int m_rbo;		// レンダーバッファオブジェクト

	unsigned int m_colorBuffers[2];
	unsigned int m_attachments[2];

	Shader* m_shader;
};