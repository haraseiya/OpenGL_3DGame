#pragma once
#include "NPCActorBase.h"
#include "BoxCollider.h"

class Player;
class EnemyBase;
class BoxCollider;
class NPCBehaviorComponent;

class Attacker : public NPCActorBase
{
public:
	Attacker(Player* player,EnemyBase* enemy);
	~Attacker();

	void UpdateActor(float deltaTime) override;
	void OnCollisionEnter(ColliderComponent* own,ColliderComponent* other) override;
	void GetDamage(const int& power);

	bool IsFrontHit();

private:
	BoxCollider* mHitBox;
	BoxCollider* mAttackTriggerBox;
	NPCBehaviorComponent* mNPCBehaviorComponent;

	static const int m_maxHP;
	static const float m_maxSpeed;

	float mCoolTime;
};