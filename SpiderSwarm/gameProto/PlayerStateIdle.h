#pragma once
#include "PlayerStateBase.h"

class PlayerBullet;

class PlayerStateIdle : public PlayerStateBase
{
public:
	PlayerStateIdle(PlayerBehaviorComponent* owner);
	~PlayerStateIdle();

	PlayerStateEnum Update(float deltaTime) override;
	void OnEnter()override;
	void OnExit()override;

	void Shot(float deltaTime);

private:
	bool mIsControllerInputOff;
	bool mIsIdle;

	SkeletalMeshComponent* mMeshComp;

	PlayerBullet* mBullet;
	float mShootTimer;
	static const float mInterval;
};