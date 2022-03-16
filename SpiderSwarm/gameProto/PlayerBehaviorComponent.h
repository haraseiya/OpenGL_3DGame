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

	void Update(float deltaTime) override;        // �X�e�[�g����
	void ChangeState(PlayerStateEnum state);       // �X�e�[�g�ύX
	void RegisterState(PlayerStateBase* state);		// �X�e�[�g�̓o�^
	void SetFirstState(PlayerStateEnum state);
	void SetState(PlayerStateEnum state) { mNextState = state; }

	bool HasState(PlayerStateEnum state);          // ���̃X�e�[�g�������Ă��邩�H
	PlayerBase* GetOwnerActor() { return static_cast<PlayerBase*>(mOwner); }

	PlayerStateEnum GetPlayerState() { return mNextState; }

protected:
	std::unordered_map<PlayerStateEnum, PlayerStateBase*> mStateMap;
	PlayerStateBase* mNowState;
	PlayerStateEnum mNextState;		// ���̃X�e�[�g
};

