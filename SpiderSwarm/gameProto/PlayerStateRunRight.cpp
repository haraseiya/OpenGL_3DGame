#include "PlayerStateRunRight.h"
#include "PlayerBehaviorComponent.h"

PlayerStateRunRight::PlayerStateRunRight(PlayerBehaviorComponent* owner)
	: PlayerStateBase(owner)
{
	mStateType = PlayerStateEnum::RunRight;
}

PlayerStateRunRight::~PlayerStateRunRight()
{
}

PlayerStateEnum PlayerStateRunRight::Update(float deltaTime)
{
	return PlayerStateEnum();
}

void PlayerStateRunRight::OnEnter()
{
}

void PlayerStateRunRight::OnExit()
{
}
