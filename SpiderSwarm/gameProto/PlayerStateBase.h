#pragma once

#include "PlayerBase.h"
#include "PlayerBehaviorComponent.h"

class PlayerStateBase
{
public:
	PlayerStateBase(PlayerBehaviorComponent* owner)
		: mOwnerComponent(owner)
		, mStateType(PlayerStateEnum::Invalid)
		, mMeshComp(nullptr)
	{
		mOwner = mOwnerComponent->GetOwnerActor();
	}

	virtual ~PlayerStateBase() {};
	virtual PlayerStateEnum Update(float deltaTime) = 0;
	virtual void OnEnter() = 0;
	virtual void OnExit() = 0;

	PlayerStateEnum         GetStateType() { return mStateType; }

protected:
	PlayerBehaviorComponent* mOwnerComponent;
	PlayerBase* mOwner;
	PlayerStateEnum mStateType;
	SkeletalMeshComponent* mMeshComp;
};