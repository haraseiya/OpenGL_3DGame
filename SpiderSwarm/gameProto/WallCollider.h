#pragma once
#include "Component.h"
#include "ColliderComponent.h"
#include "PhysicsWorld.h"
#include "Collision.h"

class WallCollider : public ColliderComponent
{
public:
	WallCollider(class GameObject* owner, int updateOrder = 100);
	~WallCollider();

	void SetWall(const Wall& wall);
	const Wall& GetWall()const;

	bool CollisionDetection(ColliderComponent* other) override;

protected:
	bool Check(BoxCollider* other)override;
	Wall mWall;

	friend class PhysicsWorld;
};
