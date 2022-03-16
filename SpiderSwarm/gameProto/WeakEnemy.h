#pragma once
#include "EnemyBase.h"
#include "BoxCollider.h"

enum class WeakEnemyType
{
	TYPE_CHASE,
	TYPE_
};

class Player;
class NPCActorBase;
class EnemyBehaviorComponent;
class BoxCollider;
class ExplosionEffect;
class EnemyBullet;

class WeakEnemy : public EnemyBase
{
public:
	WeakEnemy(GameObject* target,const Vector3& startPos);
	~WeakEnemy();

	void UpdateActor(float deltaTime) override;
	void OnCollisionEnter(ColliderComponent* own,ColliderComponent* other) override;
	void FixCollision(BoxCollider* hitPlayerBox, BoxCollider* hitBox) override;

	void SetAttackHitBox(float scale) override;
	void LoadAnimation();		// �A�j���[�V�����ǂݍ���

	//virtual EnemyBase* Clone() override;
private:
	void LoadModel();			// ���f���ǂݍ���
	void LoadSkeleton();		// �X�P���g���ǂݍ���
	void BehaviorResister();	// �U�镑���̒ǉ�
	void SetCollider() override;			// ���g�̓����蔻���ǉ�
	void RemoveCollider() override;

	BoxCollider* mBoxCollider;
	BoxCollider* mAttackTrigger;
	BoxCollider* mAttackBox;
	EnemyBehaviorComponent* mEnemyBehaviorComponent;
	GameObject* mTarget;

	ExplosionEffect* mExplosion;	// �����G�t�F�N�g

	AABB mEnemyBox;
	AABB mEnemyForward;
	AABB mEnemyAttackTrigger;

	EnemyBullet* mBullet;

	Vector3 mForwardVec; // �O�i�����x�N�g��  

	float mShootTimer;
	static const float mInterval;
	float mTimer;
};