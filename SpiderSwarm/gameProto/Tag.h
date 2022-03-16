#pragma once
#include <string>
#include <stdexcept>
#include <iterator>

// �Q�[�����A�N�^�[�E�����蔻��̎�ޕʃ^�O
// �K�v�Ȃ��̂�Begin��End�̊Ԃ�255�܂Œ�`�ł���

enum class Tag : unsigned char
{
	BEGIN,               // Tag�̊J�n�}�[�J�[�@�iBegin����O�ɒ�`���Ă͂Ȃ�Ȃ��j

	PLAYER = BEGIN,
	PLAYER_BULLET,
	PLAYER_SPECIAL_SHOT,
	ENEMY,
	ENEMY_BOSS,
	ENEMY_BULLET,
	ENEMY_ATTACK,
	BACK_GROUND,
	CAMERA,
	EFFECT,
	ITEM_EXPERIENCE,
	ITEM_HEAL,
	NONE,

	END                  // Tag�̏I���}�[�J�[�@�iEnd����ɒ�`���Ă͂Ȃ�Ȃ��j
};

// �O�u�C���N�������g
Tag& operator ++(Tag& rhs);


