#pragma once
#include "PlayerStateBase.h"

class PlayerBase;
class PlayerBullet;

class PlayerStateForward : public PlayerStateBase
{
public:
	PlayerStateForward(PlayerBehaviorComponent* owner);
	~PlayerStateForward();

	PlayerStateEnum Update(float deltaTime) override;

	void OnEnter()override;
	void OnExit()override;

	void Shot(float deltaTime);

private:
	void MoveCalc(float deltaTime);
	static const float mRoot;
	float mDot;

	PlayerBullet* mBullet;
	float mShootTimer;
	static const float mInterval;
};

