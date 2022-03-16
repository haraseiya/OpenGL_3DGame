#pragma once
#include "PlayerStateBase.h"

class PlayerStateHold : public PlayerStateBase
{
public:
	PlayerStateHold(PlayerBehaviorComponent* owner);
	~PlayerStateHold();

	PlayerStateEnum Update(float deltaTime) override;

	void OnEnter()override;
	void OnExit()override;
};

