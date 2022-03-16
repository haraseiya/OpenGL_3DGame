#pragma once

#include "EnemyState.h"

class NPCActorBase;
class EnemyBehaviorComponent;
class EnemyBase;

class EnemyAttack :public EnemyState
{
public:
	EnemyAttack(EnemyBehaviorComponent* owner);
	~EnemyAttack();

	EnemyStateEnum Update(float deltaTime) override;
	void           OnEnter() override;
	void           OnExit()  override;

private:
	const float mAnimationSpeed;
};