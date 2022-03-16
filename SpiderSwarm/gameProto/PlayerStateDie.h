#pragma once
#include "PlayerStateBase.h"

class PlayerStateDie : public PlayerStateBase
{
public:
	PlayerStateDie(PlayerBehaviorComponent* owner);
	~PlayerStateDie();

	PlayerStateEnum Update(float deltaTime) override;
	void OnEnter()override;
	void OnExit()override;

private:
	SkeletalMeshComponent* mMeshComp;
};

