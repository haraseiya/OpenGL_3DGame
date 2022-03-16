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
	// stateMap����state�N���X�S�������
	for (auto i = mStateMap.begin(); i != mStateMap.end(); ++i)
	{
		delete i->second;
	}
	std::cout << ("remove : EnemyBehaviorComponent [%d]\n", mID) << std::endl;
}

void NPCBehaviorComponent::Update(float deltaTime)
{
	// ���݂̏�Ԃ��Ȃ���Β�~
	if (!mNowState)	return;

	// ���݂̃X�e�[�g�̎��s
	NPCStateEnum nextState = mNowState->Update(deltaTime);

	// �����X�e�[�g�������玟�̃X�e�[�g�ύX�w����������X�e�[�g�ύX������	
	if (mNowState->GetStateType() != nextState)
	{
		ChangeState(nextState);
	}
}

// �X�e�[�g�ύX����
void NPCBehaviorComponent::ChangeState(NPCStateEnum state)
{
	// ���݂̏�Ԃ��甲����
	const bool isWrongState = mNowState->GetStateType() != state;
	if (isWrongState)
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
		std::cout << "���o�^�X�e�[�g�ւ̈ڍs���s���܂��� : " << GetNPCStateEnumName(mNowState->GetStateType()) << std::endl;
		mNowState = nullptr;
	}
}

// �X�e�[�g��o�^
void NPCBehaviorComponent::RegisterState(NPCState* state)
{
	// �X�e�[�g�^�C�v�ʂɓo�^
	mStateMap.emplace(state->GetStateType(), state);
}

// 
void NPCBehaviorComponent::SetFirstState(NPCStateEnum state)
{
	// �X�e�[�g�������Ă�����
	if (HasState(state))
	{
		// �X�e�[�g�}�b�v�ɓo�^���A�A�j���[�V�������Đ�
		mNowState = mStateMap[state];
		mNowState->OnEnter();
	}
	else
	{
		std::cout << "���o�^�X�e�[�g : " << GetNPCStateEnumName(state) << std::endl;
	}
}

// ���̃X�e�[�g�������Ă��邩�A�����Ă�����True
bool NPCBehaviorComponent::HasState(NPCStateEnum state)
{
	auto itr = mStateMap.find(state);
	return itr != mStateMap.end();
}
