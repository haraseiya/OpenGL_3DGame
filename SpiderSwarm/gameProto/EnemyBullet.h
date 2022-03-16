#pragma once
#include "BulletBase.h"

class EnemyBase;
class InstanceMeshComponent;

class EnemyBullet : public BulletBase
{
public:
	EnemyBullet(EnemyBase* enemy, const Vector3& direction, float scale, float speed);
	~EnemyBullet();

	void UpdateActor(float deltaTime)override;
	void OnCollisionEnter(ColliderComponent* ownCollider, ColliderComponent* otherBox)override;
	void SetCollider()override;

	//void ComputeWorldTransform()override;

private:
	Tag mTag;
	float mLifeTime;
	static const float mMaxLifeTime;
};

