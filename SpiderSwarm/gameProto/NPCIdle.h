#pragma once
#include "NPCState.h"

class NPCBehaviorComponent;
class Player;
class EnemyBase;

class NPCIdle : public NPCState
{
public:
	NPCIdle(NPCBehaviorComponent* owner,Player* player,EnemyBase* enemy);
	~NPCIdle();

	NPCStateEnum Update(float deltaTime) override;
	void           OnEnter() override;
	void           OnExit()  override;

private:
	Player* mPlayer;
	EnemyBase* mEnemy;

	Vector3 mNPCPos;
	Vector3 mPlayerPos;
	Vector3 mDistance;
};