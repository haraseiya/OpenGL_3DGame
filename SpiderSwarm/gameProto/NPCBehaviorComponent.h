#pragma once
#include "Component.h"
#include "NPCActorBase.h"
#include <unordered_map>

class NPCState;

class NPCBehaviorComponent : public Component
{
public:
	NPCBehaviorComponent(NPCActorBase* owner);
	~NPCBehaviorComponent();

	void Update(float deltaTime) override;      // ステート処理
	void ChangeState(NPCStateEnum state);       // ステート変更
	void RegisterState(class NPCState* state);  // ステートの登録
	void SetFirstState(NPCStateEnum state);		// 最初の状態をセット
	bool HasState(NPCStateEnum state);          // そのステートを持っているか？
	NPCActorBase* GetOwnerActor() { return static_cast<NPCActorBase*>(mOwner); }

protected:
	std::unordered_map<NPCStateEnum, NPCState*> mStateMap;
	NPCState* mNowState;
};