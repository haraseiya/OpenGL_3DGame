#pragma once

#include "NPCState.h"

class NPCBehaviorComponent;

class NPCDie :public NPCState
{
public:
	NPCDie(NPCBehaviorComponent* owner);
	~NPCDie();

	NPCStateEnum Update(float deltaTime) override;
	void           OnEnter() override;
	void           OnExit()  override;
};
