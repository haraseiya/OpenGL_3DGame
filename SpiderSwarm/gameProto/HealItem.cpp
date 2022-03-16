#include "HealItem.h"
#include "InstanceMeshComponent.h"
#include "BoxCollider.h"

HealItem::HealItem(const Vector3& pos)
	: GameObject(Tag::ITEM_HEAL)
	, mLifeTime(0.0f)
{
	mInstanceMeshComp = new InstanceMeshComponent(this, InstanceType::HealCapsule);

	// �����ʒu
	mPosition = pos;

	// �e�����蔻��
	AABB box;
	box.mMin = Vector3(-30, -30, -30);
	box.mMax = Vector3(30, 30, 30);
	box.mIsRotatable = false;
	BoxCollider* bc = new BoxCollider(this);
	bc->SetObjectBox(box);
}

HealItem::~HealItem()
{
}

void HealItem::UpdateActor(float deltaTime)
{
	// ��]�̍X�V
	Quaternion rot = GetRotation();
	float angle = 2.0f * deltaTime;
	Quaternion inc(Vector3::UnitZ, angle);
	rot = Quaternion::Concatenate(rot, inc);
	SetRotation(rot);

	mPosition.z = Math::Sin(deltaTime) + 800;

	// �������Ԃ��߂���Ύ��g������
	mLifeTime += deltaTime;
	const bool isDead = mLifeTime >= 10.0f;
	if (isDead)
	{
		mLifeTime = 0.0f;
		mState = State::STATE_DEAD;
	}
}

void HealItem::OnCollisionEnter(ColliderComponent* own, ColliderComponent* other)
{
	// �^�O���Q�b�g
	Tag otherTag = other->GetTag();

	// �Փ˂����̂��v���C���[�̏ꍇ
	if (otherTag == Tag::PLAYER)
	{
		// ���g���폜
		mState = State::STATE_DEAD;
	}
}
