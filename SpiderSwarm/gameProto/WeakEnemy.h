#pragma once
#include "EnemyBase.h"
#include "BoxCollider.h"

enum class WeakEnemyType
{
	TYPE_CHASE,
	TYPE_
};

class Player;
class NPCActorBase;
class EnemyBehaviorComponent;
class BoxCollider;
class ExplosionEffect;
class EnemyBullet;

class WeakEnemy : public EnemyBase
{
public:
	WeakEnemy(GameObject* target,const Vector3& startPos);
	~WeakEnemy();

	void UpdateActor(float deltaTime) override;
	void OnCollisionEnter(ColliderComponent* own,ColliderComponent* other) override;
	void FixCollision(BoxCollider* hitPlayerBox, BoxCollider* hitBox) override;

	void SetAttackHitBox(float scale) override;
	void LoadAnimation();		// アニメーション読み込み

	//virtual EnemyBase* Clone() override;
private:
	void LoadModel();			// モデル読み込み
	void LoadSkeleton();		// スケルトン読み込み
	void BehaviorResister();	// 振る舞いの追加
	void SetCollider() override;			// 自身の当たり判定を追加
	void RemoveCollider() override;

	BoxCollider* mBoxCollider;
	BoxCollider* mAttackTrigger;
	BoxCollider* mAttackBox;
	EnemyBehaviorComponent* mEnemyBehaviorComponent;
	GameObject* mTarget;

	ExplosionEffect* mExplosion;	// 爆発エフェクト

	AABB mEnemyBox;
	AABB mEnemyForward;
	AABB mEnemyAttackTrigger;

	EnemyBullet* mBullet;

	Vector3 mForwardVec; // 前進方向ベクトル  

	float mShootTimer;
	static const float mInterval;
	float mTimer;
};