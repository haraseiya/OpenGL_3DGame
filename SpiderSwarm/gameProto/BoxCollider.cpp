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
	// �{�b�N�X�R���C�_�[��PhysicsWirld�ɓo�^
	GAMEINSTANCE.GetPhysics()->AddCollider(this);
}

BoxCollider::~BoxCollider()
{
	// �{�b�N�X�R���C�_�[��PhysicsWorld����j��
	GAMEINSTANCE.GetPhysics()->RemoveCollider(this);
}

void BoxCollider::OnUpdateWorldTransform()
{
	// �I�u�W�F�N�g��Ԃ̃{�b�N�X�Ƀ��Z�b�g
	mWorldBox = mObjectBox;

	// �X�P�[�����O
	mWorldBox.mMin *= mOwner->GetScale();
	mWorldBox.mMax *= mOwner->GetScale();

	// ��]
	if (mRotatable)
	{
		mWorldBox.Rotate(mOwner->GetRotation());
	}

	//���s�ړ�
	mWorldBox.mMin += mOwner->GetPosition();
	mWorldBox.mMax += mOwner->GetPosition();

	mWorldBox.CalcVertex();
}

// AABB���Z�b�g
void BoxCollider::SetObjectBox(const AABB& box)
{
	mObjectBox = box;
	mRotatable = box.mIsRotatable;
}

// �Փˌ��o
bool BoxCollider::CollisionDetection(ColliderComponent* other)
{
	return other->Check(this);
}

// Box���m
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
