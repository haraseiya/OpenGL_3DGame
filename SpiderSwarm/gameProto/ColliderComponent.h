#pragma once
#include "Component.h"
#include "Collision.h"
#include "PhysicsWorld.h"
#include "Tag.h"

class GameObject;
class BoxCollider;
class WallCollider;

// 当たり判定の種類
enum class ColliderTypeEnum
{
	Box,
	Wall,
	Sphere,
	Line,
};

// 当たり判定を付与するコンポーネントクラス
class ColliderComponent : public Component
{
public:
	ColliderComponent(GameObject* owner, ColliderTypeEnum type, int updateOrder = 100);
	virtual ~ColliderComponent() {};

	Tag GetTag();
	void SetInfo(const CollisionInfo& info) { mInfo = info; }
	void ClearInfo();

	ColliderTypeEnum GetColliderType() { return mColliderType; }
	CollisionInfo& GetCollisionInfo() { return mInfo; }

	virtual void OnUpdateWorldTransform() {};

	// Double-Dispatch パターンを使用して衝突判定組み合わせを行う
	virtual bool CollisionDetection(ColliderComponent* other) = 0;

	// Double-Dispatch パターン：Collider同士のふるまいを定義
	virtual bool Check(BoxCollider* other);
	virtual bool Check(WallCollider* other);

protected:
	Tag              mTag;                // 当たり判定グループタグ
	ColliderTypeEnum mColliderType;       // 衝突タイプ
	CollisionInfo    mInfo;               // 衝突時情報

	friend class PhysicsWorld;
};