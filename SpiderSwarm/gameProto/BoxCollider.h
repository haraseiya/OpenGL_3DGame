#pragma once
#include "Component.h"
#include "ColliderComponent.h"
#include "Collision.h"
#include "PhysicsWorld.h"
#include "Tag.h"

class WallCollider;

class BoxCollider : public ColliderComponent
{
public:
	BoxCollider(class GameObject* owner, int updateOrder = 100);
	~BoxCollider();

	void  OnUpdateWorldTransform();								  // ワールド変換時
	void  SetObjectBox(const AABB& box);                          // あたり判定用境界ボックスをセット
	const AABB& GetWorldBox() const { return mWorldBox; }         // ワールド空間上での境界ボックスを取得
	void  SetArrowRotate(bool value) { mRotatable = value; }      // 回転を許可するか？

	bool CollisionDetection(ColliderComponent* other) ;

protected:
	bool Check(BoxCollider* other);
	bool Check(WallCollider* other);

	AABB mObjectBox;                                              // オブジェクト空間（変換前）のボックス
	AABB mWorldBox;                                               // ワールド空間に置いた時のボックス
	bool mRotatable;                                              // 回転を許可するか？

	friend class PhysicsWorld;
};