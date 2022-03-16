#pragma once
#include "PlayerStateBase.h"

class PlayerBase;

class PlayerStateRunForward : public PlayerStateBase
{
public:
	PlayerStateRunForward(PlayerBehaviorComponent* owner);
	~PlayerStateRunForward();

	PlayerStateEnum Update(float deltaTime) override;

	void OnEnter()override;
	void OnExit()override;

private:
	void MoveCalc(float deltaTime);
	static const float mRoot;
	float mDot;
};

