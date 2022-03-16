#include "EnemyRoar.h"

EnemyRoar::EnemyRoar(EnemyBehaviorComponent* owner)
	: EnemyState(owner)
{
	mStateType = EnemyStateEnum::Roar;
}

EnemyRoar::~EnemyRoar()
{
}

EnemyStateEnum EnemyRoar::Update(float deltaTime)
{
	if (mOwner->GetEnemyKind() == EnemyKind::ENEMY_BOSS)
	{
		mOwner->SetHitPoint(50000);
	}

	// 体力が0以下なら死亡状態
	const bool isDead = mOwner->GetHitPoint() <= 0;
	if (isDead)
	{
		return EnemyStateEnum::Death;
	}

	// ボス敵なら追跡アニメーションに遷移
	if (!mOwner->IsAnimationPlaying() && mOwner->GetEnemyKind() == EnemyKind::ENEMY_BOSS)
	{
		return EnemyStateEnum::Run;
	}

	// アニメーションが終了したら待機状態平行
	if (!mOwner->IsAnimationPlaying())
	{
		return EnemyStateEnum::Idle;
	}

	return EnemyStateEnum::Roar;
}

void EnemyRoar::OnEnter()
{
	mOwner->PlayAnimation(EnemyStateEnum::Roar);
}

void EnemyRoar::OnExit()
{
}
