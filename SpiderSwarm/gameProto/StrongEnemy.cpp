#include "StrongEnemy.h"
#include "PlayerBase.h"
#include "NPCActorBase.h"
#include "Mesh.h"
#include "Renderer.h"
#include "Skeleton.h"
#include "SkeletalMeshComponent.h"
#include "Texture.h"
#include "BoxCollider.h"
#include "Collision.h"
#include "Animation.h"
#include "MeshComponent.h"
#include "SpriteComponent.h"
#include "PhysicsWorld.h"
#include "ExplosionEffect.h"
#include "EnemyBullet.h"

// ���G�X�e�[�g�֘A
#include "EnemyBehaviorComponent.h"
#include "EnemyIdle.h"
#include "EnemyChase.h"
#include "EnemyAttack.h"
#include "EnemySpawn.h"
#include "EnemyDeath.h"
#include "EnemyCharge.h"

#include <iostream>

const float StrongEnemy::mInterval = 5.0f;	// �e�̔��ˊԊu

StrongEnemy::StrongEnemy(GameObject* target,const Vector3& pos)
	: mTarget(target)
{
	// �p�����[�^�[������
	mPosition = pos;
	mEnemyKind = EnemyKind::ENEMY_STRONG;
	mScale = 1.0f;
	mHitPoint = 20;
	mWalkSpeed = 500.0f;
	mRunSpeed = 500.0f;
	mTurnSpeed = Math::Pi;

	// ���f���ǂݍ���
	LoadModel();

	// �X�P���g���ǂݍ���
	LoadSkeleton();

	// �A�j���[�V�����ǂݍ���
	LoadAnimation();

	// EnemyBehavior�ɂӂ�܂���o�^
	BehaviorResister();

	//// �U���p�g���K�[�ǉ�
	//SetAttackTrigger();

	SetCollider();
}

StrongEnemy::~StrongEnemy()
{
	std::cout << "�{�X�G�j��" << std::endl;
}

void StrongEnemy::UpdateActor(float deltaTime)
{
	// ���G�f�t�H���g�̐F�ɃZ�b�g
	mSkelMeshComponent->SetHitColor(Color::Black);
}

void StrongEnemy::OnCollisionEnter(ColliderComponent* own,ColliderComponent* other)
{
	// �����Ă����^�O���擾
	Tag otherColliderTag = other->GetTag();

	// �v���C���[�e�ƏՓ˂�����
	if (otherColliderTag == Tag::PLAYER_BULLET)
	{
		// ��e�F�Z�b�g
		mSkelMeshComponent->SetHitColor(Color::Red);
		mHitPoint--;
	}

	//// �v���C���[�ɏՓˎ��⊮���Ȃ���ʒu���C��
	//if (otherColliderTag == Tag::ENEMY || otherColliderTag == Tag::PLAYER)
	//{
	//	// �C�����̈ʒu������
	//	Vector3 fix;

	//	//�ǂƂԂ������Ƃ�
	//	AABB otherEnemyBox = dynamic_cast<BoxCollider*>(other)->GetWorldBox();
	//	AABB enemyBox = mHitBox->GetWorldBox();

	//	// �߂荞�݂��C��
	//	calcCollisionFixVec(enemyBox, otherEnemyBox, fix);

	//	// �x�N�g����␳���Ȃ���߂�
	//	mPosition = Vector3::Lerp(mPosition, mPosition + fix, 0.1f);
	//	mPosition.z = 500.0f;

	//	// �ʒu�Čv�Z
	//	ComputeWorldTransform();
	//}
}

void StrongEnemy::FixCollision(BoxCollider* hitEnemyBox, BoxCollider* hitPlayerBox)
{
	// �������Ƃ��̈ʒu
	Vector3 fix;

	// �ǂƂԂ������Ƃ�
	AABB playerBox = hitPlayerBox->GetWorldBox();
	AABB enemyBox = mHitBox->GetWorldBox();

	// �߂荞�݂��C��
	calcCollisionFixVec(playerBox, enemyBox, fix);

	// �␳�x�N�g�����߂�
	mPosition += fix;

	// �ʒu���ς�����̂Ń{�b�N�X�Čv�Z
	mHitBox->OnUpdateWorldTransform();
}

// �U���p�����蔻����Z�b�g
void StrongEnemy::SetAttackHitBox(float scale)
{
	// �U������p�{�b�N�X�̐���
	mAttackBox = new BoxCollider(this);

	// �G�O�������̓����蔻��
	AABB box = mEnemyBox;
	box.mMin *= 1.5;
	box.mMax *= 1.5;
	mAttackBox->SetObjectBox(box);
}

//EnemyBase* StrongEnemy::Clone()
//{
//	return new StrongEnemy(mTarget);
//}

// �U���p�����蔻���j��
//void StrongEnemy::RemoveAttackHitBox()
//{
//	if (mAttackBox)
//	{
//		delete mAttackBox;
//		mAttackBox = nullptr;
//	}
//}

// ���g�̓����蔻���j��
//void StrongEnemy::RemoveHitBox()
//{
//	if (mHitBox)
//	{
//		delete mHitBox;
//		mHitBox = nullptr;
//	}
//}

// ���f���̃��[�h
void StrongEnemy::LoadModel()
{
	mSkelMeshComponent = new SkeletalMeshComponent(this);
	mMesh = RENDERER->GetMesh("Assets/Character/Enemy/StrongEnemy/StrongSpider.gpmesh");
}

// �X�P���g���̓ǂݍ���
void StrongEnemy::LoadSkeleton()
{
	mSkelMeshComponent->SetMesh(mMesh);
	mSkelMeshComponent->SetSkeleton(RENDERER->GetSkeleton("Assets/Character/Enemy/StrongEnemy/StrongSpider.gpskel"));
}

// �A�j���[�V�����̓ǂݍ���
void StrongEnemy::LoadAnimation()
{
	mAnimations.emplace(EnemyStateEnum::Spawn, RENDERER->GetAnimation("Assets/Character/Enemy/Animation/Spider_Spawn.gpanim", false));
	mAnimations.emplace(EnemyStateEnum::Idle, RENDERER->GetAnimation("Assets/Character/Enemy/Animation/Spider_Idle.gpanim", true));
	mAnimations.emplace(EnemyStateEnum::Attack2, RENDERER->GetAnimation("Assets/Character/Enemy/Animation/Spider_Charge.gpanim", false));
	mAnimations.emplace(EnemyStateEnum::Run, RENDERER->GetAnimation("Assets/Character/Enemy/Animation/Spider_Walk.gpanim", true));
	mAnimations.emplace(EnemyStateEnum::Death, RENDERER->GetAnimation("Assets/Character/Enemy/Animation/Spider_Death.gpanim", false));
}

// ���G�ӂ�܂��̓o�^
void StrongEnemy::BehaviorResister()
{
	// �A�j���[�V�����z��ɂӂ�܂���o�^
	mEnemyBehaviorComponent = new EnemyBehaviorComponent(this);
	mEnemyBehaviorComponent->RegisterState(new EnemySpawn(mEnemyBehaviorComponent));
	mEnemyBehaviorComponent->RegisterState(new EnemyIdle(mEnemyBehaviorComponent));
	mEnemyBehaviorComponent->RegisterState(new EnemyCharge(mEnemyBehaviorComponent));
	mEnemyBehaviorComponent->RegisterState(new EnemyChase(mEnemyBehaviorComponent, mTarget));
	mEnemyBehaviorComponent->RegisterState(new EnemyDeath(mEnemyBehaviorComponent));

	// �ŏ��̃A�j���[�V�������Z�b�g
	mEnemyBehaviorComponent->SetFirstState(EnemyStateEnum::Spawn);
}

// �����蔻��̃Z�b�g
void StrongEnemy::SetCollider()
{
	if (mHitBox) return;
	mEnemyBox = mMesh->GetCollisionBox();
	mEnemyBox.mMin.y *= 0.5f;
	mEnemyBox.mMax.y *= 0.5f;
	mHitBox = new BoxCollider(this);
	mHitBox->SetObjectBox(mEnemyBox);
	mHitBox->SetArrowRotate(true);
}

void StrongEnemy::RemoveCollider()
{
	if (mHitBox)
	{
		delete mHitBox;
		mHitBox = nullptr;
	}
}

// �U���g���K�[�̃Z�b�g
void StrongEnemy::SetAttackTrigger()
{
	mEnemyForward.mMin.x = mEnemyBox.mMax.x;
	mEnemyForward.mMin.y = mEnemyBox.mMin.y;
	mEnemyForward.mMin.z = mEnemyBox.mMin.z;
	mEnemyForward.mMax.x = mEnemyForward.mMin.x + 100.0f;
	mEnemyForward.mMax.y = mEnemyForward.mMin.y + 100.0f;
	mEnemyForward.mMax.z = mEnemyForward.mMin.z + 100.0f;
	mAttackTrigger = new BoxCollider(this);
	mAttackTrigger->SetObjectBox(mEnemyForward);

}
