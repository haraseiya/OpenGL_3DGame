#include "ExperienceItem.h"
#include "InstanceMeshComponent.h"
#include "ColliderComponent.h"
#include "BoxCollider.h"

ExperienceItem::ExperienceItem(const Vector3& pos)
	: GameObject(Tag::ITEM_EXPERIENCE)
	, mLifeTime(0.0f)
{
	mInstanceMeshComp = new InstanceMeshComponent(this,InstanceType::EnergyCapsule);

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

ExperienceItem::~ExperienceItem()
{

}

void ExperienceItem::UpdateActor(float deltaTime)
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
		mState = GameObject::State::STATE_DEAD;
	}
}

void ExperienceItem::OnCollisionEnter(ColliderComponent* own, ColliderComponent* other)
{
	// �^�O���Q�b�g
	Tag otherTag = other->GetTag();

	// �Փ˂����̂��v���C���[�̏ꍇ
	if (otherTag == Tag::PLAYER)
	{
		// ���g���폜
		mState = GameObject::State::STATE_DEAD;
	}
}
