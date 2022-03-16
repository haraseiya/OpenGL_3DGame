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
	printf("�U���A�j���[�V�����J�n\n");

	// �U���A�j���[�V�����Đ�
	mOwner->PlayAnimation(EnemyStateEnum::Attack1, mAnimationSpeed);
}

void EnemyAttack::OnExit()
{
	printf("�U���A�j���[�V�����I��\n");
}
