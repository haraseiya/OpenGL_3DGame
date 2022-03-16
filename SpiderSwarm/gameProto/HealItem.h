#pragma once
#include "GameObject.h"

class InstanceMeshComponent;

class HealItem : public GameObject
{
public:
	HealItem(const Vector3& pos);
	~HealItem();

	void UpdateActor(float deltaTime)override;
	void OnCollisionEnter(ColliderComponent* own, ColliderComponent* other);

private:
	InstanceMeshComponent* mInstanceMeshComp;
	float mLifeTime;
};

