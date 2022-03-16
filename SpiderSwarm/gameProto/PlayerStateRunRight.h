#pragma once
#include "PlayerStateBase.h"

class PlayerStateRunRight: public PlayerStateBase
{
public:
	PlayerStateRunRight(PlayerBehaviorComponent* owner);
	~PlayerStateRunRight();

	PlayerStateEnum Update(float deltaTime) override;

	void OnEnter()override;
	void OnExit()override;
};