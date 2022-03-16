#pragma once

#include <stdint.h>
#include "Math.h"

class UIManager;
class Shader;
class Texture;
class Font;

class UIScreen
{
public:
	enum UIState
	{
		ACTIVE=0,	// アクティブ状態
		CLOSING		// クロージング状態
	};

	UIScreen(UIManager* manager);
	virtual ~UIScreen();

	// 更新
	virtual void Update(float deltaTime);

	// 描画
	virtual void Draw(Shader* shader);

	virtual void ProcessInput(const uint8_t* keys);
	virtual void HandleKeyPress(int key);

	// クロージング状態にする
	void Close();

	// 状態を取得
	UIState GetState()const { return mState; }

protected:
	// テクスチャを描画するヘルパー関数
	void DrawTexture(
		Shader* shader, Texture* texture,
		const Vector2& offset = Vector2::Zero, float scale = 1.0f);

	UIManager* mManager;

	Font* mFont;
	Texture* mTexture;

	Vector2 mPosition;

	UIState mState;

};

