#pragma once
#include "Component.h"

class GameObject;
class Shader;
class Texture;

class SpriteComponent : public Component
{
public:
	SpriteComponent(GameObject* _owner, int _drawOrder = 100);
	~SpriteComponent();

	virtual void Draw(Shader* _shader);

	// ゲッターセッター //
	// テクスチャをセットし縦横の長さを計算する
	virtual void SetTexture(Texture* _texture);

	// テクスチャの横幅
	const int GetTexWidth() const { return m_textureWidth; }

	// テクスチャの縦幅
	const int GetTexHeight() const { return m_textureHeight; }

	// テクスチャの描画順
	const int GetDrawOrder() const { return m_drawOrder; }

	// 描画するかどうかを設定
	void SetVisible(bool _visible) { m_visible = _visible; }

	// 描画するかどうかを取得
	const bool GetVisible() const { return m_visible; }

protected:
	Texture* m_texture;		// テクスチャのポインタ
	int m_drawOrder;		// 描画順（数値が少ないほど早く更新される）
	int m_textureWidth;		// テクスチャの横幅
	int m_textureHeight;	// テクスチャの縦幅
	bool m_visible;			// 描画を行うか
};

