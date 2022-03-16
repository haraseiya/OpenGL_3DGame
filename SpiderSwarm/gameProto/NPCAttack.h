#pragma once

#include "NPCState.h"

class NPCBehaviorComponent;
class EnemyBase;

class NPCAttack:public NPCState
{
public:
	NPCAttack(NPCBehaviorComponent* owner,EnemyBase* enemy);
	~NPCAttack();

	NPCStateEnum Update(float deltaTime) override;
	void           OnEnter() override;
	void           OnExit()  override;

private:
	EnemyBase* m_enemy;
};