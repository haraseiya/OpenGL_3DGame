#include "PlayerBehaviorComponent.h"
#include "PlayerStateBase.h"

#include <iostream>

PlayerBehaviorComponent::PlayerBehaviorComponent(PlayerBase* owner)
	: Component(owner)
	, mNowState(nullptr)
{
}

PlayerBehaviorComponent::~PlayerBehaviorComponent()
{
	// stateMap内のstateクラス全解放処理
	for (auto i = mStateMap.begin(); i != mStateMap.end(); ++i)
	{
		delete i->second;
	}
}

void PlayerBehaviorComponent::Update(float deltaTime)
{
	// ステートが存在しなければ
	if (!mNowState)
	{
		return;
	}

	// 現在のステートの実行
	PlayerStateEnum nextState = mNowState->Update(deltaTime);

	// もしステート内部から次のステート変更指示が来たらステート変更処理へ	
	if (mNowState->GetStateType() != nextState)
	{
		ChangeState(nextState);
	}
}

// ステート変更処理
void PlayerBehaviorComponent::ChangeState(PlayerStateEnum state)
{
	// 現在の状態から抜ける
	if (mNowState)
	{
		mNowState->OnExit();
	}

	// 新しい状態を連想配列から探す
	auto iter = mStateMap.find(state);
	if (iter != mStateMap.end())
	{
		mNowState = iter->second;

		// 新しい状態に入る
		mNowState->OnEnter();
	}
	else
	{
		//GameObject* go = GetOwnerActor();
		//if (go == nullptr)
		//{
		//	printf("存在します\n");
		//}

		//const char* name = GetOwnerActor()->GetPlayerStateEnumName(mNowState->GetStateType());
		//printf(name);

		std::cout << "未登録ステートへの移行が行われました : " << GetOwnerActor()->GetPlayerStateEnumName(mNowState->GetStateType()) << std::endl;
		mNowState = nullptr;
	}
}

// ステートを登録
void PlayerBehaviorComponent::RegisterState(PlayerStateBase* state)
{
	// ステートタイプ別に登録
	mStateMap.emplace(state->GetStateType(), state);
}

// 最初のステートを登録
void PlayerBehaviorComponent::SetFirstState(PlayerStateEnum state)
{
	// ステートを持っていたら
	if (HasState(state))
	{
		mNowState = mStateMap[state];
		mNowState->OnEnter();
	}
	else
	{
		std::cout << "未登録ステート : " << GetOwnerActor()->GetPlayerStateEnumName(state) << std::endl;
	}
}

// そのステートを持っているか
bool PlayerBehaviorComponent::HasState(PlayerStateEnum state)
{
	auto itr = mStateMap.find(state);
	return itr != mStateMap.end();
}
