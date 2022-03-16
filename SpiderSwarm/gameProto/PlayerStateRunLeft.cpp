#include "PlayerStateRunLeft.h"
#include "PlayerBehaviorComponent.h"

PlayerStateRunLeft::PlayerStateRunLeft(PlayerBehaviorComponent* owner)
	: PlayerStateBase(owner)
{
	mStateType = PlayerStateEnum::RunLeft;
}

PlayerStateRunLeft::~PlayerStateRunLeft()
{
}

PlayerStateEnum PlayerStateRunLeft::Update(float deltaTime)
{
	return PlayerStateEnum();
}

void PlayerStateRunLeft::OnEnter()
{
}

void PlayerStateRunLeft::OnExit()
{
}
