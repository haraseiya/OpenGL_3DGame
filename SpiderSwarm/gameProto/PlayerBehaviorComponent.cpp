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
	// stateMap����state�N���X�S�������
	for (auto i = mStateMap.begin(); i != mStateMap.end(); ++i)
	{
		delete i->second;
	}
}

void PlayerBehaviorComponent::Update(float deltaTime)
{
	// �X�e�[�g�����݂��Ȃ����
	if (!mNowState)
	{
		return;
	}

	// ���݂̃X�e�[�g�̎��s
	PlayerStateEnum nextState = mNowState->Update(deltaTime);

	// �����X�e�[�g�������玟�̃X�e�[�g�ύX�w����������X�e�[�g�ύX������	
	if (mNowState->GetStateType() != nextState)
	{
		ChangeState(nextState);
	}
}

// �X�e�[�g�ύX����
void PlayerBehaviorComponent::ChangeState(PlayerStateEnum state)
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
		//GameObject* go = GetOwnerActor();
		//if (go == nullptr)
		//{
		//	printf("���݂��܂�\n");
		//}

		//const char* name = GetOwnerActor()->GetPlayerStateEnumName(mNowState->GetStateType());
		//printf(name);

		std::cout << "���o�^�X�e�[�g�ւ̈ڍs���s���܂��� : " << GetOwnerActor()->GetPlayerStateEnumName(mNowState->GetStateType()) << std::endl;
		mNowState = nullptr;
	}
}

// �X�e�[�g��o�^
void PlayerBehaviorComponent::RegisterState(PlayerStateBase* state)
{
	// �X�e�[�g�^�C�v�ʂɓo�^
	mStateMap.emplace(state->GetStateType(), state);
}

// �ŏ��̃X�e�[�g��o�^
void PlayerBehaviorComponent::SetFirstState(PlayerStateEnum state)
{
	// �X�e�[�g�������Ă�����
	if (HasState(state))
	{
		mNowState = mStateMap[state];
		mNowState->OnEnter();
	}
	else
	{
		std::cout << "���o�^�X�e�[�g : " << GetOwnerActor()->GetPlayerStateEnumName(state) << std::endl;
	}
}

// ���̃X�e�[�g�������Ă��邩
bool PlayerBehaviorComponent::HasState(PlayerStateEnum state)
{
	auto itr = mStateMap.find(state);
	return itr != mStateMap.end();
}
