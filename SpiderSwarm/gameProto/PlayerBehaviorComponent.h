#pragma once

#include "PlayerBase.h"
#include "Component.h"
#include <unordered_map>

class PlayerBase;
class PlayerStateBase;

class PlayerBehaviorComponent : public Component
{
public:
	PlayerBehaviorComponent(PlayerBase* player);
	~PlayerBehaviorComponent();

	void Update(float deltaTime) override;        // ステート処理
	void ChangeState(PlayerStateEnum state);       // ステート変更
	void RegisterState(PlayerStateBase* state);		// ステートの登録
	void SetFirstState(PlayerStateEnum state);
	void SetState(PlayerStateEnum state) { mNextState = state; }

	bool HasState(PlayerStateEnum state);          // そのステートを持っているか？
	PlayerBase* GetOwnerActor() { return static_cast<PlayerBase*>(mOwner); }

	PlayerStateEnum GetPlayerState() { return mNextState; }

protected:
	std::unordered_map<PlayerStateEnum, PlayerStateBase*> mStateMap;
	PlayerStateBase* mNowState;
	PlayerStateEnum mNextState;		// 次のステート
};

