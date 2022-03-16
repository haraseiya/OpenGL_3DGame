#pragma once

#include "NPCState.h"
#include "NPCBehaviorComponent.h"

class Player;
class EnemyBase;

enum Mode
{
	Chase,	// ’Ç]ƒ‚[ƒh
	Attack	// UŒ‚ƒ‚[ƒh
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

	Vector3 m_npcForwardVec;	// NPC‚Ì‘O•û•ûŒü‚ğŠi”[
	Vector3 m_npcPos;			// NPC‚ÌˆÊ’u‚ğŠi”[
	Vector3 m_playerPos;		// ƒvƒŒƒCƒ„[‚ÌˆÊ’u‚ğŠi”[
	Vector3 m_enemyPos;			// “G‚ÌˆÊ’u‚ğŠi”[
	Vector3 m_direction;		// Œü‚«î•ñ‚ğŠi”[
	Vector3 mDistance;
	Vector3 mRange;// ”ÍˆÍ‚æ‚è‹ß‚¯‚ê‚Î‘Ò‹@‚ÉˆÚs

	float m_speed;				// ‘¬“x
	Mode m_mode;				

	float distanceX;
	float distanceY;
};
