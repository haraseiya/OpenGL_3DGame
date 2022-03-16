#include "HealItem.h"
#include "InstanceMeshComponent.h"
#include "BoxCollider.h"

HealItem::HealItem(const Vector3& pos)
	: GameObject(Tag::ITEM_HEAL)
	, mLifeTime(0.0f)
{
	mInstanceMeshComp = new InstanceMeshComponent(this, InstanceType::HealCapsule);

	// 初期位置
	mPosition = pos;

	// 弾当たり判定
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
	// 回転の更新
	Quaternion rot = GetRotation();
	float angle = 2.0f * deltaTime;
	Quaternion inc(Vector3::UnitZ, angle);
	rot = Quaternion::Concatenate(rot, inc);
	SetRotation(rot);

	mPosition.z = Math::Sin(deltaTime) + 800;

	// 生存期間を過ぎれば自身を消す
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
	// タグをゲット
	Tag otherTag = other->GetTag();

	// 衝突したのがプレイヤーの場合
	if (otherTag == Tag::PLAYER)
	{
		// 自身を削除
		mState = State::STATE_DEAD;
	}
}
