#pragma once
#include "EnemyBase.h"
#include "EnemyBehaviorComponent.h"

class EnemyState
{
public:
	EnemyState(EnemyBehaviorComponent* owner)
		: mOwnerComponent(owner)
		, mStateType(EnemyStateEnum::Invalid)
	{
		mOwner = mOwnerComponent->GetOwnerActor();
	}

	//��Ԃ��Ƃ̂ӂ�܂�
	virtual EnemyStateEnum Update(float deltaTime) = 0;
	virtual void           OnEnter() = 0;
	virtual void           OnExit() = 0;

	EnemyStateEnum         GetStateType() { return mStateType; }

protected:
	EnemyBehaviorComponent* mOwnerComponent;
	EnemyBase* mOwner;
	EnemyStateEnum mStateType;
};