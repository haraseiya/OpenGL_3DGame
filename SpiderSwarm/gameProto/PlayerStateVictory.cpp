#include "PlayerStateVictory.h"
#include "SkeletalMeshComponent.h"

PlayerStateVictory::PlayerStateVictory(PlayerBehaviorComponent* owner)
	: PlayerStateBase(owner)
{
	mStateType = PlayerStateEnum::Victory;
}

PlayerStateVictory::~PlayerStateVictory()
{
}

PlayerStateEnum PlayerStateVictory::Update(float deltaTime)
{
	return PlayerStateEnum::Victory;
}

void PlayerStateVictory::OnEnter()
{
	// アイドル状態のアニメーション再生
	mMeshComp = mOwner->GetSkeletalMeshComp();
	mMeshComp->PlayAnimation(mOwner->GetAnim(PlayerState::PLAYER_STATE_VICTORY), 0.5f);
}

void PlayerStateVictory::OnExit()
{
}
