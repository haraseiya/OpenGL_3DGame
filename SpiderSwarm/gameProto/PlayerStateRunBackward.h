#pragma once
#include "PlayerStateBase.h"
class PlayerStateRunBackward :public PlayerStateBase
{
public:
	PlayerStateRunBackward(PlayerBehaviorComponent* owner);
	~PlayerStateRunBackward();

	PlayerStateEnum Update(float deltaTime) override;

	void OnEnter()override;
	void OnExit()override;
};

