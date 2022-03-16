#pragma once
#include "EnemyBase.h"
#include "BoxCollider.h"

class Player;
class NPCActorBase;
class EnemyBehaviorComponent;
class BoxCollider;
class ExplosionEffect;
class EnemyBullet;

class StrongEnemy : public EnemyBase
{
public:
	StrongEnemy(GameObject* target,const Vector3& pos);
	~StrongEnemy();

	void UpdateActor(float deltaTime) override;
	void OnCollisionEnter(ColliderComponent* own,ColliderComponent* other) override;
	void FixCollision(BoxCollider* hitPlayerBox, BoxCollider* hitBox) override;

	void SetAttackHitBox(float scale) override;

	//virtual EnemyBase* Clone() override;

private:
	void LoadModel();			// モデル読み込み
	void LoadSkeleton();		// スケルトン読み込み
	void LoadAnimation();		// アニメーション読み込み
	void BehaviorResister();	// 振る舞いの追加
	void SetCollider() override;			// 自身の当たり判定を追加
	void RemoveCollider() override;
	void SetAttackTrigger();	// アタックトリガーを追加

	BoxCollider* mAttackTrigger;
	BoxCollider* mAttackBox;
	EnemyBehaviorComponent* mEnemyBehaviorComponent;
	GameObject* mTarget;
	ExplosionEffect* mExplosion;
	//EnemyBullet* mEnemyBullet;

	AABB mEnemyBox;
	AABB mEnemyForward;
	AABB mEnemyAttackTrigger;

	float mShootTimer;
	static const float mInterval;


};