#include "EnemyAttack.h"
#include "NPCActorBase.h"
#include "BossEnemy.h"

EnemyAttack::EnemyAttack(EnemyBehaviorComponent* owner)
	: EnemyState(owner)
	, mAnimationSpeed(0.5f)
{
	mStateType = EnemyStateEnum::Attack1;
}

EnemyAttack::~EnemyAttack()
{
}

EnemyStateEnum EnemyAttack::Update(float deltaTime)
{
	return EnemyStateEnum::Attack1;
}

void EnemyAttack::OnEnter()
{
	printf("攻撃アニメーション開始\n");

	// 攻撃アニメーション再生
	mOwner->PlayAnimation(EnemyStateEnum::Attack1, mAnimationSpeed);
}

void EnemyAttack::OnExit()
{
	printf("攻撃アニメーション終了\n");
}
