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
		ACTIVE=0,	// �A�N�e�B�u���
		CLOSING		// �N���[�W���O���
	};

	UIScreen(UIManager* manager);
	virtual ~UIScreen();

	// �X�V
	virtual void Update(float deltaTime);

	// �`��
	virtual void Draw(Shader* shader);

	virtual void ProcessInput(const uint8_t* keys);
	virtual void HandleKeyPress(int key);

	// �N���[�W���O��Ԃɂ���
	void Close();

	// ��Ԃ��擾
	UIState GetState()const { return mState; }

protected:
	// �e�N�X�`����`�悷��w���p�[�֐�
	void DrawTexture(
		Shader* shader, Texture* texture,
		const Vector2& offset = Vector2::Zero, float scale = 1.0f);

	UIManager* mManager;

	Font* mFont;
	Texture* mTexture;

	Vector2 mPosition;

	UIState mState;

};

