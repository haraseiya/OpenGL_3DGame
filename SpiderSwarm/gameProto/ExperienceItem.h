#pragma once
#include "GameObject.h"

class InstanceMeshComponent;

class ExperienceItem : public GameObject
{
public:
	ExperienceItem(const Vector3& pos);
	~ExperienceItem();

	void UpdateActor(float deltaTime)override;
	void OnCollisionEnter(ColliderComponent* own, ColliderComponent* other);

private:
	InstanceMeshComponent* mInstanceMeshComp;
	float mLifeTime;
};

