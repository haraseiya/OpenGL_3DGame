#pragma once

#include "NPCState.h"
#include "NPCBehaviorComponent.h"

class Player;
class EnemyBase;

enum Mode
{
	Chase,	// �Ǐ]���[�h
	Attack	// �U�����[�h
};

class NPCRun : public NPCState
{
public:
	NPCRun(class NPCBehaviorComponent* owner, Player* player,EnemyBase* enemy);
	virtual ~NPCRun();
	NPCStateEnum Update(float deltaTime) override;
	void           OnEnter() override;
	void           OnExit()  override;
	Mode GetMode() { return m_mode; }

private:
	Player* m_player;
	EnemyBase* m_enemy;

	Vector3 m_npcForwardVec;	// NPC�̑O���������i�[
	Vector3 m_npcPos;			// NPC�̈ʒu���i�[
	Vector3 m_playerPos;		// �v���C���[�̈ʒu���i�[
	Vector3 m_enemyPos;			// �G�̈ʒu���i�[
	Vector3 m_direction;		// ���������i�[
	Vector3 mDistance;
	Vector3 mRange;// �͈͂��߂���Αҋ@�Ɉڍs

	float m_speed;				// ���x
	Mode m_mode;				

	float distanceX;
	float distanceY;
};
