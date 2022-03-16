#include "EnemySpawn.h"

EnemySpawn::EnemySpawn(EnemyBehaviorComponent* owner)
	: EnemyState(owner)
{
	mStateType = EnemyStateEnum::Spawn;
}

EnemySpawn::~EnemySpawn()
{
}

EnemyStateEnum EnemySpawn::Update(float deltaTime)
{
	if (mOwner->GetEnemyKind() == EnemyKind::ENEMY_WEAK)
	{
		mOwner->SetHitPoint(5);
	}
	if (mOwner->GetEnemyKind() == EnemyKind::ENEMY_STRONG)
	{
		mOwner->SetHitPoint(20);
	}

	// アニメーション終了時
	if (!mOwner->IsAnimationPlaying())
	{
		return EnemyStateEnum::Idle;
	}

	return EnemyStateEnum::Spawn;
}

void EnemySpawn::OnEnter()
{
	// 出現アニメーション再生
	mOwner->PlayAnimation(EnemyStateEnum::Spawn);
}

void EnemySpawn::OnExit()
{

}

