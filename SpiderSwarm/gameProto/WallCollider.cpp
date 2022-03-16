#include "WallCollider.h"
#include "Collision.h"
#include "Game.h"
#include "PhysicsWorld.h"
#include "BoxCollider.h"

WallCollider::WallCollider(GameObject* owner, int updateOrder)
	: ColliderComponent(owner, ColliderTypeEnum::Wall, updateOrder)
	, mWall()
{
	GAMEINSTANCE.GetPhysics()->AddCollider(this);
}

WallCollider::~WallCollider()
{
	printf("remove WallCollider : [%5d] owner->(0x%p)\n", GetID(), mOwner);
	GAMEINSTANCE.GetPhysics()->RemoveCollider(this);
}

void WallCollider::SetWall(const Wall& wall)
{
	mWall = wall;
}

const Wall& WallCollider::GetWall() const
{
	return mWall;
}

// Õ“ËŒŸo
bool WallCollider::CollisionDetection(ColliderComponent* other)
{
	return other->Check(this);
}

// Wall vs Box
bool WallCollider::Check(BoxCollider* other)
{
	CollisionInfo info;
	if (Intersect(other->GetWorldBox(), mWall, info))
	{
		other->SetInfo(info);
		return true;
	}

	return false;
}
