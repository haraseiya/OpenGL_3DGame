#include "GameObject.h"
#include "Collision.h"
#include "BoxCollider.h"
#include "Game.h"
#include "PhysicsWorld.h"
#include "WallCollider.h"

BoxCollider::BoxCollider(GameObject* owner, int updateOrder)
	: ColliderComponent(owner, ColliderTypeEnum::Box, updateOrder)
	, mObjectBox(Vector3::Zero, Vector3::Zero)
	, mWorldBox(Vector3::Zero, Vector3::Zero)
	, mRotatable(true)
{
	// ボックスコライダーをPhysicsWirldに登録
	GAMEINSTANCE.GetPhysics()->AddCollider(this);
}

BoxCollider::~BoxCollider()
{
	// ボックスコライダーをPhysicsWorldから破棄
	GAMEINSTANCE.GetPhysics()->RemoveCollider(this);
}

void BoxCollider::OnUpdateWorldTransform()
{
	// オブジェクト空間のボックスにリセット
	mWorldBox = mObjectBox;

	// スケーリング
	mWorldBox.mMin *= mOwner->GetScale();
	mWorldBox.mMax *= mOwner->GetScale();

	// 回転
	if (mRotatable)
	{
		mWorldBox.Rotate(mOwner->GetRotation());
	}

	//平行移動
	mWorldBox.mMin += mOwner->GetPosition();
	mWorldBox.mMax += mOwner->GetPosition();

	mWorldBox.CalcVertex();
}

// AABBをセット
void BoxCollider::SetObjectBox(const AABB& box)
{
	mObjectBox = box;
	mRotatable = box.mIsRotatable;
}

// 衝突検出
bool BoxCollider::CollisionDetection(ColliderComponent* other)
{
	return other->Check(this);
}

// Box同士
bool BoxCollider::Check(BoxCollider* other)
{
	return Intersect(other->GetWorldBox(), GetWorldBox());
}

// Box vs Wall
bool BoxCollider::Check(WallCollider* other)
{
	CollisionInfo info;
	if (Intersect(GetWorldBox(), other->GetWall(), info))
	{
		SetInfo(info);
		return true;
	}
	return false;
}
