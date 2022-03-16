#include "PlayerStateRunBackward.h"

PlayerStateRunBackward::PlayerStateRunBackward(PlayerBehaviorComponent* owner)
	: PlayerStateBase(owner)
{
	mStateType = PlayerStateEnum::RunBackward;
}

PlayerStateRunBackward::~PlayerStateRunBackward()
{
}

PlayerStateEnum PlayerStateRunBackward::Update(float deltaTime)
{
	return PlayerStateEnum();
}

void PlayerStateRunBackward::OnEnter()
{
}

void PlayerStateRunBackward::OnExit()
{
}
