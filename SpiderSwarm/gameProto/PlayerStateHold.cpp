#include "PlayerStateHold.h"
#include "SkeletalMeshComponent.h"

PlayerStateHold::PlayerStateHold(PlayerBehaviorComponent* owner)
	: PlayerStateBase(owner)
{
	mStateType = PlayerStateEnum::Hold;
}

PlayerStateHold::~PlayerStateHold()
{
}

PlayerStateEnum PlayerStateHold::Update(float deltaTime)
{
	return PlayerStateEnum::Hold;
}

void PlayerStateHold::OnEnter()
{
	mMeshComp = mOwner->GetSkeletalMeshComp();
	mMeshComp->PlayAnimation(mOwner->GetAnim(PlayerState::PLAYER_STATE_HOLD), 0.5f);
}

void PlayerStateHold::OnExit()
{
}
