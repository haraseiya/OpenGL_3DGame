#include "EnemyBullet.h"
#include "EnemyBase.h"
#include "InstanceMeshComponent.h"
#include "BoxCollider.h"

const float EnemyBullet::mMaxLifeTime = 20.0f;

EnemyBullet::EnemyBullet(EnemyBase* enemy,const Vector3& direction,float scale,float speed)
	: BulletBase(Tag::ENEMY_BULLET,InstanceType::EnemyBullet1)
{
	// �p�����[�^�[������
	mPosition = enemy->GetPosition();
	mPosition.z = enemy->GetPosition().z + 50;
	mDirection = direction;
	mScale = scale;
	mSpeed = speed;
	mLifeTime = 0.0f;

	// �����蔻��̃Z�b�g
	SetCollider();
	RotateToNewForward(mDirection);
}

EnemyBullet::~EnemyBullet()
{
}

void EnemyBullet::UpdateActor(float deltaTime)
{
	mLifeTime += deltaTime;

	// �������Ԃ��߂���Ύ��g������
	//const bool isDead = mLifeTime >= mMaxLifeTime;
	//if (isDead)
	//{
	//	mLifeTime = 0.0f;
	//	mState = STATE_DEAD;
	//}

	// �ʒu�̍X�V
	mPosition += mSpeed * deltaTime * mDirection;
	mPosition.z = 800.0f;

	// ��]�̍X�V
	Quaternion rot = GetRotation();
	float angle = 5.0f * deltaTime;
	Quaternion inc(mDirection, angle);
	rot = Quaternion::Concatenate(rot, inc);
	SetRotation(rot);

	mRecomputeWorldTransform = true;
}

void EnemyBullet::OnCollisionEnter(ColliderComponent* ownCollider, ColliderComponent* otherBox)
{
	// �^�O���Q�b�g
	Tag otherTag = otherBox->GetTag();

	if (otherTag == Tag::PLAYER|| otherTag == Tag::BACK_GROUND)
	{
		mState = GameObject::State::STATE_DEAD;
	}
}

void EnemyBullet::SetCollider()
{
	// �e�����蔻��
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
