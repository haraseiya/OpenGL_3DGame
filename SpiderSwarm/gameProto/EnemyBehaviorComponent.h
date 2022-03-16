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

	void Update(float deltaTime) override;        // �X�e�[�g����
	void ChangeState(EnemyStateEnum state);       // �X�e�[�g�ύX
	void RegisterState(EnemyState* state);		// �X�e�[�g�̓o�^
	void SetFirstState(EnemyStateEnum state);
	void SetState(EnemyStateEnum state) { mNextState = state; }

	bool HasState(EnemyStateEnum state);          // ���̃X�e�[�g�������Ă��邩�H
	EnemyBase* GetOwnerActor() { return static_cast<EnemyBase*>(mOwner); }

protected:
	std::unordered_map<EnemyStateEnum, EnemyState*> mStateMap;
	EnemyState* mNowState;
	EnemyStateEnum mNextState;		// ���̃X�e�[�g
};