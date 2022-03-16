#pragma once

#include "EnemyState.h"

class EnemyBehaviorComponent;

class EnemyIdle :public EnemyState
{
public:
	EnemyIdle(EnemyBehaviorComponent* owner);
	~EnemyIdle();

	EnemyStateEnum Update(float deltaTime) override;

	void           OnEnter() override;
	void           OnExit()  override;

private:
	GameObject* mTarget;
	int mNextAnimType;
};

