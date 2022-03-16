#pragma once
#include <string>
#include <stdexcept>
#include <iterator>

// ゲーム内アクター・当たり判定の種類別タグ
// 必要なものはBeginとEndの間に255個まで定義できる

enum class Tag : unsigned char
{
	BEGIN,               // Tagの開始マーカー　（Beginより手前に定義してはならない）

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

	END                  // Tagの終了マーカー　（Endより後に定義してはならない）
};

// 前置インクリメント
Tag& operator ++(Tag& rhs);


