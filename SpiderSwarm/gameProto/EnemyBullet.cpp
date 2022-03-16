#include "EnemyBullet.h"
#include "EnemyBase.h"
#include "InstanceMeshComponent.h"
#include "BoxCollider.h"

const float EnemyBullet::mMaxLifeTime = 20.0f;

EnemyBullet::EnemyBullet(EnemyBase* enemy,const Vector3& direction,float scale,float speed)
	: BulletBase(Tag::ENEMY_BULLET,InstanceType::EnemyBullet1)
{
	// パラメーター初期化
	mPosition = enemy->GetPosition();
	mPosition.z = enemy->GetPosition().z + 50;
	mDirection = direction;
	mScale = scale;
	mSpeed = speed;
	mLifeTime = 0.0f;

	// 当たり判定のセット
	SetCollider();
	RotateToNewForward(mDirection);
}

EnemyBullet::~EnemyBullet()
{
}

void EnemyBullet::UpdateActor(float deltaTime)
{
	mLifeTime += deltaTime;

	// 生存期間を過ぎれば自身を消す
	//const bool isDead = mLifeTime >= mMaxLifeTime;
	//if (isDead)
	//{
	//	mLifeTime = 0.0f;
	//	mState = STATE_DEAD;
	//}

	// 位置の更新
	mPosition += mSpeed * deltaTime * mDirection;
	mPosition.z = 800.0f;

	// 回転の更新
	Quaternion rot = GetRotation();
	float angle = 5.0f * deltaTime;
	Quaternion inc(mDirection, angle);
	rot = Quaternion::Concatenate(rot, inc);
	SetRotation(rot);

	mRecomputeWorldTransform = true;
}

void EnemyBullet::OnCollisionEnter(ColliderComponent* ownCollider, ColliderComponent* otherBox)
{
	// タグをゲット
	Tag otherTag = otherBox->GetTag();

	if (otherTag == Tag::PLAYER|| otherTag == Tag::BACK_GROUND)
	{
		mState = GameObject::State::STATE_DEAD;
	}
}

void EnemyBullet::SetCollider()
{
	// 弾当たり判定
	AABB box;
	box.mMin = Vector3(-30, -30, -30);
	box.mMax = Vector3(30, 30, 30);
	box.mIsRotatable = false;
	BoxCollider* bc = new BoxCollider(this);
	bc->SetObjectBox(box);
}

//void EnemyBullet::ComputeWorldTransform()
//{
//
//}
