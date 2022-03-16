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

	// ������/�I������
	void Initialize();
	void Finalize();

	// �t�H���g�̃��[�h�E�A�����[�h
	bool Load(const std::string& fileName);
	void UnLoad();

	// ��������e�N�X�`���ɕ`��
	Texture* RenderText(const std::string& _text,
						const Vector3& _color = Color::White,
						int _pointSize = 72);

private:
	// �t�H���g�T�C�Y�ƃt�H���g�f�[�^�̘A�z�z��
	std::unordered_map<int, TTF_Font*> mFontData;

	UIManager* mUIManager;
};

