#pragma once

#include "EffectBase.h"
#include "Collision.h"

#include <iostream>

class GameObject;
class BoxCollider;

class LaserEffect : public EffectBase
{
public:
	LaserEffect(GameObject* owner);
	virtual ~LaserEffect();

	void UpdateActor(float deltaTime) override;
	void OnCollisionEnter(ColliderComponent* ownCollider, ColliderComponent* otherBox) override;

	void LoadEffect()override;
	void CreateEffect()override;

private:
	AABB mLaserBox;
	BoxCollider* mHitBox;
	GameObject* mOwner;
};

