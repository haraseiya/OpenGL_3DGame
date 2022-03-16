#include "PlayerBullet.h"
#include "Game.h"
#include "BoxCollider.h"
#include "EffectComponent.h"
#include "ExplosionEffect.h"
#include "Renderer.h"
#include "MeshComponent.h"
#include "InstanceMeshComponent.h"
#include "InstanceMeshManager.h"
#include "PlayerBase.h"
#include "ObjectPool.h"

const float PlayerBullet::mTurnShotInterval = 0.5f;
const float PlayerBullet::mLifeTime = 2.0f;

PlayerBullet::PlayerBullet(PlayerBase* player,const Vector3& pos, const Vector3& dir, float scale, float speed)
	: BulletBase(Tag::PLAYER_BULLET,InstanceType::PlayerBullet1)
	, mOwner(player)
	, mShotType(ShotType::SHOT_TURN)
{
	// �p�����[�^�[������
	mPosition = pos;
	mPosition.z = pos.z + 100.0f;
	mDirection = dir;
	mScale = scale;
	mSpeed = speed;
	mLifeTimer = 0.0f;

	mVelocityX = mSpeed * Math::Cos(Math::Pi * 2 * mDirection.x);
	mVelocityY = mSpeed * Math::Sin(Math::Pi * 2 * mDirection.y);

	// �R���C�_�[�̃Z�b�g
	SetCollider();

	RotateToNewForward(mDirection);
}

PlayerBullet::~PlayerBullet()
{
}

void PlayerBullet::UpdateActor(float deltaTime)
{
	// �������Ԃ��߂���Ύ��g������
	mLifeTimer += deltaTime;
	const bool isDead = mLifeTimer >= mLifeTime;
	if (isDead)
	{
		mLifeTimer = 0.0f;
		mState = GameObject::State::STATE_DEAD;
	}

	// �v���C���[�̃��x�����擾
	int level = mOwner->GetLevel();
	/*if (level >= 1 && level<4) */mShotType = ShotType::SHOT_NORMAL;
	//if (level >= 4 && level < 8)mShotType = ShotType::SHOT_TURN;

	// �V���b�g�^�C�v�ɂ���ԑJ��
	switch (mShotType)
	{
	// �����V���b�g
	case ShotType::SHOT_NORMAL:
		NormalMove(deltaTime);
		break;

	// ����V���b�g
	case ShotType::SHOT_TURN:
		//TurnMove(deltaTime);
		break;

	case ShotType::SHOT_HOMING:
		HomingMove(deltaTime);
		break;
	}

}

void PlayerBullet::OnCollisionEnter(ColliderComponent* ownCollider, ColliderComponent* otherBox)
{
	// �^�O���Q�b�g
	Tag otherTag = otherBox->GetTag();

	// �Փ˂����̂��G�̏ꍇ
	if (/*otherTag == Tag::ENEMY||*/ otherTag == Tag::BACK_GROUND)
	{
		// �e���폜
		mState = GameObject::State::STATE_DEAD;
	}
}

void PlayerBullet::SetCollider()
{
	// �e�����蔻��
	AABB box;
	box.mMin = Vector3(-30, -30, -30);
	box.mMax = Vector3(30, 30, 30);
	box.mIsRotatable = false;
	BoxCollider* bc = new BoxCollider(this);
	bc->SetObjectBox(box);
}

void PlayerBullet::NormalMove(float deltaTime)
{
	mPosition += mSpeed * deltaTime * mDirection;
	
	mRecomputeWorldTransform = true;
}

void PlayerBullet::TurnMove(float deltaTime)
{
	
	//mTurnShotTime += deltaTime;
	////// ���g�̈ʒu���X�V
	////GameObject* target = GAMEINSTANCE.GetEnemyActor();

	////// �^�[�Q�b�g�����݂��Ȃ����
	////if (!target) return;

	////Vector3 direction = target->GetPosition() - mPosition;
	////direction.Normalize();
	////mPosition += 1000 * deltaTime * direction;

	//mPosition.x += mVelocityX * deltaTime;
	//mPosition.y += mVelocityY * deltaTime;
	//mPosition.z = 780.0f;

	//// �V���b�g�\�Ȃ��
	//const bool isShot = mTurnShotTime > mTurnShotInterval;
	//if (isShot)
	//{
	//	mTurnShotTime = 0.0f;
	//	float rad = Math::Atan2(target->GetPosition().y - mPosition.y, target->GetPosition().x - mPosition.x);
	//	mVelocityX = mSpeed * Math::Cos(rad);
	//	mVelocityY = mSpeed * Math::Sin(rad);
	//}
	//mRecomputeWorldTransform = true;
}

void PlayerBullet::HomingMove(float deltaTime)
{
	//GameObject* target = GAMEINSTANCE.GetEnemyActor();

	//// �^�[�Q�b�g�̕��������߂�
	//float newrad = Math::Atan2(target->GetPosition().y - mPosition.y, target->GetPosition().x - mPosition.x);

	//// �^�[�Q�b�g�����Ɛi�s�����̍����s���Ĕ͈͂Ȃ�Βe���^�[�Q�b�g�Ɍ�����
	//if(Math::Abs(newrad-))
}
