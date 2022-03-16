#pragma once
#include "GameObject.h"

// 依存クラス
class Player;
class WeakEnemy;
class SkeletalMeshComponent;
class SpriteComponent;
class BoxCollider;
class Animation;
class Mesh;
struct Sphere;

class Clasher :public GameObject
{
public:
	// NPCのアニメーション種類
	enum Anim
	{
		CLASHER_IDLE,		// 待機アニメーション
		CLASHER_RUN,		// 走りアニメーション
		CLASHER_ITEMNUM		// 総アニメーション数
	};

	// どの陣営所属か
	enum Camp
	{
		NONE,		// どちら側でもない
		PLAYER,		// プレイヤー陣営
		ENEMY,		// エネミー陣営
	};

	// NPCの状態
	enum State
	{
		WAIT,		// 待つ
		FOLLOW,		// 追従
		ATTACK,		// 攻撃
	};

	Clasher();

	// 状態を変える
	void ChangeState(Player& _player, WeakEnemy& _enemy);

	// 追従する
	void Follow(Player& _player, WeakEnemy& _enemy);

	// 状態ゲッターセッター
	const State GetNPCState() { return m_state; }
	const void SetNPCState(State _state) { m_state = _state; }

	// 陣営のゲッターセッター
	const Camp GetCamp() { return m_camp; }
	void SetCamp(Camp _camp) { m_camp = _camp; }

	// HPのゲッターセッター
	const int GetHP() { return m_hp; }
	const void SetHP(int _hp) { m_hp = _hp; }

	// 特有の更新処理
	void UpdateActor(float _deltaTime)override;

private:
	SkeletalMeshComponent* mMeshComp;
	SpriteComponent* m_spriteComp;
	Mesh* m_mesh;
	BoxCollider* m_hitBox;
	BoxCollider* mHitGroundBox;
	BoxCollider* mHitHeadBox;
	std::vector<BoxCollider*>   mPlayerBoxs;                 // プレーヤーあたりデータ
	std::vector<BoxCollider*>   mNPCBoxs;
	Sphere* m_collision;

	Camp m_camp;
	State m_state;
	Anim m_animState;
	std::vector<const Animation*> m_animTypes;

	float m_speed;
	int m_hp;
};