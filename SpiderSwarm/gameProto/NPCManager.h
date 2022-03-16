#pragma once
#include "GameObject.h"

class NPCActorBase;
class Player;
class EnemyBase;

class NPCManager
{
public:
	NPCManager(Player* player,EnemyBase* enemy,int npcNum);
	~NPCManager();

	// �X�V����
	void Update(Player* player,EnemyBase* enemy,float deltaTime);

	// �����̌v�Z
	const float& CalcDistance(NPCActorBase* npc1, NPCActorBase* npc2);

	// ��ԋ߂�NPC�̌v�Z
	NPCActorBase* ClosestNPC(std::vector<NPCActorBase*> npcs);

	NPCActorBase* GetExistNPC();

private:
	// �N���X�֘A
	std::vector<NPCActorBase*> mNPC;

	static const int m_maxNPCNum;

	float m_x;
	float m_y;
	float time;
	std::vector<float> mDistance;
};
