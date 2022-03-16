#pragma once

#include "EnemyState.h"
#include "EnemyBehaviorComponent.h"

class PlayerBase;
class NPCActorBase;
class EnemyBehaviorComponent;
class EnemyBase;
class EnemyBullet;

class EnemyChase: public EnemyState
{
public:
	EnemyChase(EnemyBehaviorComponent* owner,GameObject* target);
	virtual ~EnemyChase();
	EnemyStateEnum Update(float deltaTime) override;
	void           OnEnter() override;
	void           OnExit()  override;

private:
	void WeakEnemyMove();
	void StrongEnemyMove();
	void BossEnemyMove();

	PlayerBase* mPlayer;	// �v���C���[�̃|�C���^
	GameObject* mTarget;	// �^�[�Q�b�g�̃|�C���^
	EnemyBase* mEnemy;		// �G�̃|�C���^
	EnemyBullet* mEnemyBullet;

	static const float mRange;
	static const float mInterval;
	float mDistance;
	float mTime;
	float mShootTimer;
	float mAttackInterval;
	float mDeltaTime;
};
