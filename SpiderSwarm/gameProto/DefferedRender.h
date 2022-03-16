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
	unsigned int m_gPosition;	// 位置用カラーバッファ
	unsigned int m_gNormal;		// 法線用カラーバッファ
	unsigned int m_gAlbedoSpec;	// アルベド（RGB）&スペキュラー（A）用カラーバッファ
	unsigned int m_gRBO;		// レンダーバッファ

	unsigned int m_attachments[3];

	// バッファーテクスチャ
	std::vector<unsigned int>m_gBufferTexture;
};

