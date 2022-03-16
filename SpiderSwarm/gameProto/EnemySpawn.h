#pragma once

#include "EnemyState.h"

class EnemySpawn : public EnemyState
{
public:
	EnemySpawn(EnemyBehaviorComponent* owner);
	~EnemySpawn();

	EnemyStateEnum Update(float deltaTime) override;
	void           OnEnter() override;
	void           OnExit()  override;
};

