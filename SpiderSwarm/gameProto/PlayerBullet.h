#pragma once

#include "BulletBase.h"

class PlayerBase;
class Mesh;
class SkeletalMeshComponent;
class InstanceMeshComponent;
class ObjectPool;


enum class ShotType:unsigned char
{
	SHOT_NORMAL = 0,	// ���ʂ̃V���b�g
	SHOT_TURN,			// ����V���b�g
	SHOT_HOMING			// �z�[�~���O�V���b�g
};


class PlayerBullet :public BulletBase
{
public:
	PlayerBullet(PlayerBase* player,const Vector3& pos, const Vector3& dir, float scale, float speed);
	~PlayerBullet();

	void UpdateActor(float deltaTime)override;
	void OnCollisionEnter(ColliderComponent* ownCollider, ColliderComponent* otherBox)override;

	void SetCollider();

	void NormalMove(float deltaTime);	// ���ʂ̒e��
	void TurnMove(float deltaTime);		// ����e
	void HomingMove(float deltaTime);

private:
	PlayerBase* mOwner;
	ShotType mShotType;		// �V���b�g�̎��

	float mLifeTimer;		// �e�̐�������

	float mTurnShotTime;
	static const float mTurnShotInterval;
	static const float mLifeTime;

	float mVelocityX;	// X����
	float mVelocityY;	// Y����
};

