#include "EnemyBehaviorComponent.h"
#include "EnemyState.h"

#include <iostream>

EnemyBehaviorComponent::EnemyBehaviorComponent(EnemyBase* owner)
	: Component(owner)
	, mNowState(nullptr)
{
}

EnemyBehaviorComponent::~EnemyBehaviorComponent()
{
	// stateMap����state�N���X�S�������
	for (auto i = mStateMap.begin(); i != mStateMap.end(); ++i)
	{
		delete i->second;
	}
}

void EnemyBehaviorComponent::Update(float deltaTime)
{
	// �X�e�[�g�����݂��Ȃ����
	if (!mNowState)
	{
		return;
	}

	// ���݂̃X�e�[�g�̎��s
	mNextState = mNowState->Update(deltaTime);

	// ���݂̏�ԂƎ��̏�Ԃ��Ⴆ��
	const bool isNextState = mNowState->GetStateType() != mNextState;
	if (isNextState)
	{
		ChangeState(mNextState);
	}
}

// �X�e�[�g�ύX����
void EnemyBehaviorComponent::ChangeState(EnemyStateEnum state)
{
	// ���݂̏�Ԃ��甲����
	if (mNowState)
	{
		mNowState->OnExit();
	}

	// �V������Ԃ�A�z�z�񂩂�T��
	auto iter = mStateMap.find(state);
	if (iter != mStateMap.end())
	{
		mNowState = iter->second;

		// �V������Ԃɓ���
		mNowState->OnEnter();
	}
	else
	{
		std::cout << "���o�^�X�e�[�g�ւ̈ڍs���s���܂��� : " << GetOwnerActor()->GetEnemyStateEnumName(mNowState->GetStateType()) << std::endl;
		mNowState = nullptr;
	}
}

// �X�e�[�g��o�^
void EnemyBehaviorComponent::RegisterState(EnemyState* state)
{
	// �X�e�[�g�^�C�v�ʂɓo�^
	mStateMap.emplace(state->GetStateType(), state);
}

void EnemyBehaviorComponent::SetFirstState(EnemyStateEnum state)
{
	// �X�e�[�g�������Ă�����
	if (HasState(state))
	{
		mNowState = mStateMap[state];
		mNowState->OnEnter();
	}
	else
	{
		std::cout << "���o�^�X�e�[�g : " << GetOwnerActor()->GetEnemyStateEnumName(state) << std::endl;
	}
}

// ���̃X�e�[�g�������Ă��邩
bool EnemyBehaviorComponent::HasState(EnemyStateEnum state)
{
	auto itr = mStateMap.find(state);
	return itr != mStateMap.end();
}
