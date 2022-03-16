#pragma once

#include "EnemyState.h"

class EnemyBehaviorComponent;

class EnemyRoar :public EnemyState
{
public:
	EnemyRoar(EnemyBehaviorComponent* owner);
	~EnemyRoar();

	EnemyStateEnum Update(float deltaTime) override;

	void           OnEnter() override;
	void           OnExit()  override;
};