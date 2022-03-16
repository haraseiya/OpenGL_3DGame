#include "WeakEnemy.h"
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
#include "InstanceMeshComponent.h"
#include "EnemyBullet.h"

// �G���G�X�e�[�g�֘A
#include "EnemyBehaviorComponent.h"
#include "EnemyIdle.h"
#include "EnemyChase.h"
#include "EnemyAttack.h"
#include "EnemySpawn.h"
#include "EnemyDeath.h"
#include "EnemyRoar.h"
#include "EnemyCharge.h"

#include <iostream>

const float WeakEnemy::mInterval = 2.0f;

WeakEnemy::WeakEnemy(GameObject* target,const Vector3& startPos)
	: mShootTimer(0.0f)
	, mTarget(target)
	, mBullet(nullptr)
{
	// �p�����[�^�[������
	mPosition = startPos;
	mEnemyKind = EnemyKind::ENEMY_WEAK;
	mEnemyStateScene = EnemyStateScene::ENEMY_SCENE_TITLE;
	mScale = 0.5f; 
	mHitPoint = 5;
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

	SetCollider();
}

WeakEnemy::~WeakEnemy()
{
	std::cout << "�G���G�j��" << std::endl;
}

void WeakEnemy::UpdateActor(float deltaTime)
{
	mTimer += deltaTime;

	// �G���G��{�F
	mSkelMeshComponent->SetHitColor(Color::Black);
}

// �����蔻��
void WeakEnemy::OnCollisionEnter(ColliderComponent* own,ColliderComponent* other)
{
	// �Փ˂����I�u�W�F�N�g�̃^�O���擾
	Tag otherColliderTag = other->GetTag();

	// �v���C���[�e�ƏՓ˂�����
	if (otherColliderTag == Tag::PLAYER_BULLET)
	{
		// ��e�F�Z�b�g
		mSkelMeshComponent->SetHitColor(Color::Red);
		mHitPoint--;
	}

	// �w�i�ƏՓ˂����ꍇ
	if (otherColliderTag == Tag::BACK_GROUND)
	{
		// �C�����̈ʒu���i�[
		Vector3 fix;

		//�ǂƂԂ������Ƃ�
		AABB enemyBox = mBoxCollider->GetWorldBox();
		AABB bgBox = dynamic_cast<BoxCollider*>(other)->GetWorldBox();

		// �߂荞�݂��C��
		calcCollisionFixVec(enemyBox, bgBox, fix);
	}

	// �G���v���C���[�ɏՓ˂����ꍇ
	if (otherColliderTag == Tag::ENEMY || otherColliderTag == Tag::PLAYER)
	{
		// �C�����̈ʒu������
		Vector3 fix;

		//�ǂƂԂ������Ƃ�
		AABB otherEnemyBox = dynamic_cast<BoxCollider*>(other)->GetWorldBox();
		AABB enemyBox = mHitBox->GetWorldBox();

		// �߂荞�݂��C��
		calcCollisionFixVec(enemyBox, otherEnemyBox, fix);

		// �x�N�g����␳���Ȃ���߂�
		mPosition = Vector3::Lerp(mPosition, mPosition + fix, 0.01f);
		mPosition.z = 750.0f;

		// �ʒu�Čv�Z
		ComputeWorldTransform();
	}


	// �v���C���[�X�y�V�����V���b�g�ƏՓˎ�
	if (otherColliderTag == Tag::PLAYER_SPECIAL_SHOT)
	{
		mHitPoint -= 10;
	}
}

void WeakEnemy::FixCollision(BoxCollider* enemy, BoxCollider* player)
{
}

void WeakEnemy::SetAttackHitBox(float scale)
{
	//// �U������p�{�b�N�X�̐���
	//mAttackBox = new BoxCollider(this);

	//// �G�O�������̓����蔻��
	//AABB box = mEnemyBox;
	//box.mMin *= 1.5;
	//box.mMax *= 1.5;
	//mAttackBox->SetObjectBox(box);
}

//EnemyBase* WeakEnemy::Clone()
//{
//	//return new WeakEnemy(mTarget);
//	return nullptr;
//}

//void WeakEnemy::RemoveAttackHitBox()
//{
//	if (mAttackBox)
//	{
//		delete mAttackBox;
//		mAttackBox = nullptr;
//	}
//}

//void WeakEnemy::RemoveHitBox()
//{
//	if (mHitBox)
//	{
//		delete mHitBox;
//		mHitBox = nullptr;
//	}
//}

// ���f���̓ǂݍ���
void WeakEnemy::LoadModel()
{
	mMesh = RENDERER->GetMesh("Assets/Character/Enemy/WeakEnemy/WeakSpider.gpmesh");
	Mesh* mesh = RENDERER->GetMesh("Assets/Character/Enemy/WeakEnemy/EnemyMark.gpmesh");
}

// �{�[���̓ǂݍ���
void WeakEnemy::LoadSkeleton()
{
	mSkelMeshComponent = new SkeletalMeshComponent(this);
	mSkelMeshComponent->SetMesh(mMesh);
	mSkelMeshComponent->SetSkeleton(RENDERER->GetSkeleton("Assets/Character/Enemy/WeakEnemy/WeakSpider.gpskel"));
}

// �A�j���[�V�����̓ǂݍ���
void WeakEnemy::LoadAnimation()
{
	mEnemyBehaviorComponent = new EnemyBehaviorComponent(this);

	// �V�[���Ŏg�p����A�j���[�V�����ǂݍ���
	switch (mEnemyStateScene)
	{
	// �^�C�g���V�[��
	case EnemyStateScene::ENEMY_SCENE_TITLE:
		mAnimations.emplace(EnemyStateEnum::Roar, RENDERER->GetAnimation("Assets/Character/Enemy/Animation/Spider_Roar.gpanim", false));
		mAnimations.emplace(EnemyStateEnum::Idle, RENDERER->GetAnimation("Assets/Character/Enemy/Animation/Spider_Idle.gpanim", false));
		mEnemyBehaviorComponent->RegisterState(new EnemyRoar(mEnemyBehaviorComponent));
		mEnemyBehaviorComponent->RegisterState(new EnemyIdle(mEnemyBehaviorComponent));
		mEnemyBehaviorComponent->SetFirstState(EnemyStateEnum::Roar);
		break;

	// �Q�[���V�[��
	case EnemyStateScene::ENEMY_SCENE_GAME:
		mAnimations.emplace(EnemyStateEnum::Spawn, RENDERER->GetAnimation("Assets/Character/Enemy/BossEnemy/BossSpider_Spawn.gpanim", false));
		//mAnimations.emplace(EnemyStateEnum::Idle, RENDERER->GetAnimation("Assets/Character/Enemy/Animation/Spider_Idle.gpanim", false));
		mAnimations.emplace(EnemyStateEnum::Run, RENDERER->GetAnimation("Assets/Character/Enemy/Animation/Spider_Walk.gpanim", true));
		mAnimations.emplace(EnemyStateEnum::Attack2, RENDERER->GetAnimation("Assets/Character/Enemy/Animation/Spider_Charge.gpanim", false));
		mAnimations.emplace(EnemyStateEnum::Death, RENDERER->GetAnimation("Assets/Character/Enemy/Animation/Spider_Death.gpanim", false));
		mEnemyBehaviorComponent->RegisterState(new EnemySpawn(mEnemyBehaviorComponent));
		mEnemyBehaviorComponent->RegisterState(new EnemyIdle(mEnemyBehaviorComponent));
		mEnemyBehaviorComponent->RegisterState(new EnemyChase(mEnemyBehaviorComponent, mTarget));
		mEnemyBehaviorComponent->RegisterState(new EnemyCharge(mEnemyBehaviorComponent));
		mEnemyBehaviorComponent->RegisterState(new EnemyDeath(mEnemyBehaviorComponent));
		mEnemyBehaviorComponent->SetFirstState(EnemyStateEnum::Spawn);
		break;
	}
}

void WeakEnemy::BehaviorResister()
{


}

// �����蔻����Z�b�g
void WeakEnemy::SetCollider()
{
	if (mHitBox) return;
	mEnemyBox = mMesh->GetCollisionBox();
	mEnemyBox.mMin.y *= 0.5f;
	mEnemyBox.mMax.y *= 0.5f;
	mHitBox = new BoxCollider(this);
	mHitBox->SetObjectBox(mEnemyBox);
	mHitBox->SetArrowRotate(true);
}

// �����蔻����폜
void WeakEnemy::RemoveCollider()
{
	if (mHitBox)
	{
		delete mHitBox;
		mHitBox = nullptr;
	}
}


