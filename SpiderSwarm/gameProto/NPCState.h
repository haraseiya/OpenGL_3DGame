#pragma once
#include "NPCActorBase.h"
#include "NPCBehaviorComponent.h"

class NPCState
{
public:
	NPCState(class NPCBehaviorComponent* owner)
		: mOwnerComponent(owner)
		, mStateType(NPCStateEnum::Invalid)
	{
		mOwnerActor = mOwnerComponent->GetOwnerActor();
	}
	virtual ~NPCState() {};

	//èÛë‘Ç≤Ç∆ÇÃÇ”ÇÈÇ‹Ç¢
	virtual NPCStateEnum Update(float deltaTime) = 0;
	virtual void           OnEnter() = 0;
	virtual void           OnExit() = 0;

	NPCStateEnum        GetStateType() { return mStateType; }
	void				SetStateType(NPCStateEnum state) { mStateType = state; }

protected:
	class NPCBehaviorComponent* mOwnerComponent;
	class NPCActorBase* mOwnerActor;
	NPCStateEnum                mStateType;

};