#pragma once
#include "PlayerStateBase.h"

class PlayerStateRunLeft : public PlayerStateBase
{
public:
	PlayerStateRunLeft(PlayerBehaviorComponent* owner);
	~PlayerStateRunLeft();

	PlayerStateEnum Update(float deltaTime) override;

	void OnEnter()override;
	void OnExit()override;
};

