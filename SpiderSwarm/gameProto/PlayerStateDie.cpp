#include "PlayerStateDie.h"
#include "SkeletalMeshComponent.h"

PlayerStateDie::PlayerStateDie(PlayerBehaviorComponent* owner)
	: PlayerStateBase(owner)
{
	mStateType = PlayerStateEnum::Die;
}

PlayerStateDie::~PlayerStateDie()
{
}

PlayerStateEnum PlayerStateDie::Update(float deltaTime)
{
	if (!mMeshComp->IsPlaying())
	{
		return PlayerStateEnum::Idle;
	}

	return PlayerStateEnum::Die;
}

void PlayerStateDie::OnEnter()
{
	mMeshComp = mOwner->GetSkeletalMeshComp();
	mMeshComp->PlayAnimation(mOwner->GetAnim(PlayerState::PLAYER_STATE_DIE));
}

void PlayerStateDie::OnExit()
{
	// ���S�A�j���[�V�����t���O
	mOwner->SetDeadAnimFlag(true);
}
