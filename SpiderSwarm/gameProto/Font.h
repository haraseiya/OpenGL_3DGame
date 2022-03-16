#pragma once
#include <iostream>
#include <unordered_map>
#include <SDL/SDL_ttf.h>
#include "Math.h"

class UIManager;
class Texture;

class Font
{
public:
	Font(UIManager* manager);
	~Font();

	// 初期化/終了処理
	void Initialize();
	void Finalize();

	// フォントのロード・アンロード
	bool Load(const std::string& fileName);
	void UnLoad();

	// 文字列をテクスチャに描画
	Texture* RenderText(const std::string& _text,
						const Vector3& _color = Color::White,
						int _pointSize = 72);

private:
	// フォントサイズとフォントデータの連想配列
	std::unordered_map<int, TTF_Font*> mFontData;

	UIManager* mUIManager;
};

