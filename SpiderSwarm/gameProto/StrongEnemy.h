#pragma once
#include "EnemyBase.h"
#include "BoxCollider.h"

class Player;
class NPCActorBase;
class EnemyBehaviorComponent;
class BoxCollider;
class ExplosionEffect;
class EnemyBullet;

class StrongEnemy : public EnemyBase
{
public:
	StrongEnemy(GameObject* target,const Vector3& pos);
	~StrongEnemy();

	void UpdateActor(float deltaTime) override;
	void OnCollisionEnter(ColliderComponent* own,ColliderComponent* other) override;
	void FixCollision(BoxCollider* hitPlayerBox, BoxCollider* hitBox) override;

	void SetAttackHitBox(float scale) override;

	//virtual EnemyBase* Clone() override;

private:
	void LoadModel();			// ���f���ǂݍ���
	void LoadSkeleton();		// �X�P���g���ǂݍ���
	void LoadAnimation();		// �A�j���[�V�����ǂݍ���
	void BehaviorResister();	// �U�镑���̒ǉ�
	void SetCollider() override;			// ���g�̓����蔻���ǉ�
	void RemoveCollider() override;
	void SetAttackTrigger();	// �A�^�b�N�g���K�[��ǉ�

	BoxCollider* mAttackTrigger;
	BoxCollider* mAttackBox;
	EnemyBehaviorComponent* mEnemyBehaviorComponent;
	GameObject* mTarget;
	ExplosionEffect* mExplosion;
	//EnemyBullet* mEnemyBullet;

	AABB mEnemyBox;
	AABB mEnemyForward;
	AABB mEnemyAttackTrigger;

	float mShootTimer;
	static const float mInterval;


};