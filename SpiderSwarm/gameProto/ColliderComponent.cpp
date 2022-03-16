#include "ColliderComponent.h"
#include "GameObject.h"

ColliderComponent::ColliderComponent(GameObject* owner, ColliderTypeEnum type, int updateOrder)
    : Component(owner)
    , mColliderType(type)
{
    // タグを取得
    mTag = GetOwner()->GetTag();
}

Tag ColliderComponent::GetTag()
{
    return mTag;
}

void ColliderComponent::ClearInfo()
{
    mInfo.mFixVec = Vector3(0, 0, 0);
    mInfo.mCollisionPoint = Vector3(0, 0, 0);
}

// 当たり判定組み合わせを定義しないときはfalse返す
// 組み合わせ定義は継承先でoverrideする
bool ColliderComponent::Check(BoxCollider* other)
{
    return false;
}

bool ColliderComponent::Check(WallCollider* other)
{
    return false;
}
