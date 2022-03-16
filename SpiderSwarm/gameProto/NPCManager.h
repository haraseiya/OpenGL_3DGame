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

	// 更新処理
	void Update(Player* player,EnemyBase* enemy,float deltaTime);

	// 距離の計算
	const float& CalcDistance(NPCActorBase* npc1, NPCActorBase* npc2);

	// 一番近いNPCの計算
	NPCActorBase* ClosestNPC(std::vector<NPCActorBase*> npcs);

	NPCActorBase* GetExistNPC();

private:
	// クラス関連
	std::vector<NPCActorBase*> mNPC;

	static const int m_maxNPCNum;

	float m_x;
	float m_y;
	float time;
	std::vector<float> mDistance;
};
