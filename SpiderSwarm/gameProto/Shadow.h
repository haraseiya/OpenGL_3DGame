#pragma once

#include <GL/glew.h>
#include <string>

class Shadow
{
public:
	Shadow(const std::string& _vert, const std::string& _frag);
	~Shadow();

	// 画面全体を覆う頂点定義
	void screenVAOSetting(unsigned int& vao);

	// 影の更新
	void Update();

private:
	// シェーダー
	class Shader* m_shader;
	
	// テクスチャクラス
	//class Texture* m_texture;

	// 影の解像度
	static const int SHADOW_WIDTH;
	static const int SHADOW_HEIGHT;

	// フレームバッファ用
	unsigned int m_depthMapFBO;

	// スクリーン全体を覆う頂点バッファオブジェクト
	unsigned int m_vao;

	// テクスチャ用
	unsigned int m_texture;

	// ライト空間用行列
	Matrix4 m_lightProjection;
	Matrix4 m_lightView;
	Matrix4 m_lightSpace;
};