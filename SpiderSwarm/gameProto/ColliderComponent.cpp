#include "ColliderComponent.h"
#include "GameObject.h"

ColliderComponent::ColliderComponent(GameObject* owner, ColliderTypeEnum type, int updateOrder)
    : Component(owner)
    , mColliderType(type)
{
    // �^�O���擾
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

// �����蔻��g�ݍ��킹���`���Ȃ��Ƃ���false�Ԃ�
// �g�ݍ��킹��`�͌p�����override����
bool ColliderComponent::Check(BoxCollider* other)
{
    return false;
}

bool ColliderComponent::Check(WallCollider* other)
{
    return false;
}
