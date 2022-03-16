#include "NPCBehaviorComponent.h"
#include "NPCState.h"

#include <iostream>

NPCBehaviorComponent::NPCBehaviorComponent(NPCActorBase* owner)
	: Component(owner)
	, mNowState(nullptr)
{
	std::cout << ("Create : EnemyBehaviorComponent [%d]\n", mID) << std::endl;
}

NPCBehaviorComponent::~NPCBehaviorComponent()
{
	// stateMap内のstateクラス全解放処理
	for (auto i = mStateMap.begin(); i != mStateMap.end(); ++i)
	{
		delete i->second;
	}
	std::cout << ("remove : EnemyBehaviorComponent [%d]\n", mID) << std::endl;
}

void NPCBehaviorComponent::Update(float deltaTime)
{
	// 現在の状態がなければ停止
	if (!mNowState)	return;

	// 現在のステートの実行
	NPCStateEnum nextState = mNowState->Update(deltaTime);

	// もしステート内部から次のステート変更指示が来たらステート変更処理へ	
	if (mNowState->GetStateType() != nextState)
	{
		ChangeState(nextState);
	}
}

// ステート変更処理
void NPCBehaviorComponent::ChangeState(NPCStateEnum state)
{
	// 現在の状態から抜ける
	const bool isWrongState = mNowState->GetStateType() != state;
	if (isWrongState)
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
		std::cout << "未登録ステートへの移行が行われました : " << GetNPCStateEnumName(mNowState->GetStateType()) << std::endl;
		mNowState = nullptr;
	}
}

// ステートを登録
void NPCBehaviorComponent::RegisterState(NPCState* state)
{
	// ステートタイプ別に登録
	mStateMap.emplace(state->GetStateType(), state);
}

// 
void NPCBehaviorComponent::SetFirstState(NPCStateEnum state)
{
	// ステートを持っていたら
	if (HasState(state))
	{
		// ステートマップに登録し、アニメーションを再生
		mNowState = mStateMap[state];
		mNowState->OnEnter();
	}
	else
	{
		std::cout << "未登録ステート : " << GetNPCStateEnumName(state) << std::endl;
	}
}

// そのステートを持っているか、持っていたらTrue
bool NPCBehaviorComponent::HasState(NPCStateEnum state)
{
	auto itr = mStateMap.find(state);
	return itr != mStateMap.end();
}
