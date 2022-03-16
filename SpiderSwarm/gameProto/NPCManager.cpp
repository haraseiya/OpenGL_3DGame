#include "NPCManager.h"
#include "NPCActorBase.h"
#include "Attacker.h"
#include "EnemyBase.h"
#include "Math.h"

#include <algorithm>

// NPC‚Ì”‰Šú‰»
const int NPCManager::m_maxNPCNum = 30;

NPCManager::NPCManager(Player* player, EnemyBase* enemy,int npcNum)
{
	for (int i = 0; i < npcNum; i++)
	{
		mNPC.push_back(new Attacker(player, enemy));
		m_x= Math::GetRandom(-1000,1000);
		m_y = Math::GetRandom(-1000, 1000);
		mNPC[i]->SetPosition(Vector3(m_x, m_y, 0));
	}
}

NPCManager::~NPCManager()
{
}

void NPCManager::Update(Player* player,EnemyBase* enemy,float deltaTime)
{
}

// NPC“¯m‚Ì‹——£‚ğŒvZ
const float& NPCManager::CalcDistance(NPCActorBase* npc1, NPCActorBase* npc2)
{
	return Math::Sqrt(
		(npc1->GetPosition().x - npc2->GetPosition().x) * (npc1->GetPosition().x - npc2->GetPosition().x) +
		(npc1->GetPosition().y - npc2->GetPosition().y) * (npc1->GetPosition().y - npc2->GetPosition().y) +
		(npc1->GetPosition().z - npc2->GetPosition().z) * (npc1->GetPosition().z - npc2->GetPosition().z));
}

// ˆê”Ô‹ß‚¢NPC‚ğ•Ô‚·
NPCActorBase* NPCManager::ClosestNPC(std::vector<NPCActorBase*> npcs)
{
	for (int i = 0; i < npcs.size() - 1; i++)
	{
		for (int j = i + 1; j < npcs.size() - 1 ; j++)
		{
			mDistance.push_back(CalcDistance(npcs[i], npcs[j]));
		}
	};
	std::sort(mDistance.begin(), mDistance.end());

	return npcs[0];
}

NPCActorBase* NPCManager::GetExistNPC()
{
	return mNPC[0];
}


