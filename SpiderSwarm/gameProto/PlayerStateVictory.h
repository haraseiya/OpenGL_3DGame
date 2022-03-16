#pragma once
#include "PlayerStateBase.h"

class PlayerStateVictory : public PlayerStateBase
{
public:
	PlayerStateVictory(PlayerBehaviorComponent* owner);
	~PlayerStateVictory();

	PlayerStateEnum Update(float deltaTime) override;

	void OnEnter()override;
	void OnExit()override;
};

