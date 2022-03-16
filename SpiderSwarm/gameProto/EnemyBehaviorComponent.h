#pragma once
#include "EnemyBase.h"
#include "Component.h"
#include <unordered_map>

class EnemyBase;
class EnemyState;

class EnemyBehaviorComponent : public Component
{
public:
	EnemyBehaviorComponent(EnemyBase* owner);
	~EnemyBehaviorComponent();

	void Update(float deltaTime) override;        // ステート処理
	void ChangeState(EnemyStateEnum state);       // ステート変更
	void RegisterState(EnemyState* state);		// ステートの登録
	void SetFirstState(EnemyStateEnum state);
	void SetState(EnemyStateEnum state) { mNextState = state; }

	bool HasState(EnemyStateEnum state);          // そのステートを持っているか？
	EnemyBase* GetOwnerActor() { return static_cast<EnemyBase*>(mOwner); }

protected:
	std::unordered_map<EnemyStateEnum, EnemyState*> mStateMap;
	EnemyState* mNowState;
	EnemyStateEnum mNextState;		// 次のステート
};