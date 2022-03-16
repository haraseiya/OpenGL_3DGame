#pragma once

#include "NPCState.h"

class NPCBehaviorComponent;

class NPCHit: public NPCState
{
public:
	NPCHit(NPCBehaviorComponent* owner);
	~NPCHit();

	NPCStateEnum Update(float deltaTime)override;
	void OnEnter()override;
	void OnExit()override;

private:
	float mElapseTime;
	float mTotalAnimTime;
};