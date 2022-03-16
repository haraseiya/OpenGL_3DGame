#pragma once
#include "NPCState.h"
#include "NPCBehaviorComponent.h"

class NPCLookAround : public NPCState
{
public:
	NPCLookAround(class NPCBehaviorComponent* owner);
	~NPCLookAround();
	NPCStateEnum Update(float deltaTime) override;
	void           OnEnter() override;
	void           OnExit()  override;

};