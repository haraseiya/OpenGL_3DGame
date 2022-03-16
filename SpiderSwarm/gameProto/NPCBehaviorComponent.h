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

	void Update(float deltaTime) override;      // �X�e�[�g����
	void ChangeState(NPCStateEnum state);       // �X�e�[�g�ύX
	void RegisterState(class NPCState* state);  // �X�e�[�g�̓o�^
	void SetFirstState(NPCStateEnum state);		// �ŏ��̏�Ԃ��Z�b�g
	bool HasState(NPCStateEnum state);          // ���̃X�e�[�g�������Ă��邩�H
	NPCActorBase* GetOwnerActor() { return static_cast<NPCActorBase*>(mOwner); }

protected:
	std::unordered_map<NPCStateEnum, NPCState*> mStateMap;
	NPCState* mNowState;
};