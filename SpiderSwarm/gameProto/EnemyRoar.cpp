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

	// �̗͂�0�ȉ��Ȃ玀�S���
	const bool isDead = mOwner->GetHitPoint() <= 0;
	if (isDead)
	{
		return EnemyStateEnum::Death;
	}

	// �{�X�G�Ȃ�ǐՃA�j���[�V�����ɑJ��
	if (!mOwner->IsAnimationPlaying() && mOwner->GetEnemyKind() == EnemyKind::ENEMY_BOSS)
	{
		return EnemyStateEnum::Run;
	}

	// �A�j���[�V�������I��������ҋ@��ԕ��s
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
