#include "BossEnemy.h"
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

// �{�X�G�X�e�[�g�֘A
#include "EnemyBehaviorComponent.h"
#include "EnemyIdle.h"
#include "EnemyChase.h"
#include "EnemyAttack.h"
#include "EnemySpawn.h"
#include "EnemyDeath.h"
#include "EnemyRoar.h"
#include "EnemyCharge.h"

#include <iostream>

BossEnemy::BossEnemy(GameObject* target,const Vector3& pos)
	: mCoolTime(0.0f)
	, mTarget(target)
{
	// �p�����[�^�[������
	mPosition = pos;
	mEnemyKind = EnemyKind::ENEMY_BOSS;
	mScale = 3.0f;
	mWalkSpeed = 1000.0f;
	mRunSpeed = 500.0f;
	mTurnSpeed = Math::Pi;
	mHitPoint = 500000;

	// ���f���ǂݍ���
	LoadModel();

	// �X�P���g���ǂݍ���
	LoadSkeleton();

	// �A�j���[�V�����ǂݍ���
	LoadAnimation();

	// EnemyBehavior�ɂӂ�܂���o�^
	BehaviorResister();

	// �U���p�g���K�[�ǉ�
	SetAttackTrigger();

	SetCollider();
}

BossEnemy::~BossEnemy()
{
	std::cout << "�{�X�G�j��" << std::endl;
}

void BossEnemy::UpdateActor(float _deltaTime)
{
	// �{�X�G�f�t�H���g�F�ɃZ�b�g
	mSkelMeshComponent->SetHitColor(Color::Black);

	// HP��0�ɂȂ����玀�S
	if (mHitPoint <= 0)
	{
		mIsDead = true;
	}
	mCoolTime += _deltaTime;
}

void BossEnemy::OnCollisionEnter(ColliderComponent* own,ColliderComponent* other)
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

	// �G�ƏՓ˂�����
	if (otherColliderTag == Tag::ENEMY)
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
		mPosition.z = 500.0f;

		// �ʒu�Čv�Z
		ComputeWorldTransform();
	}
}

void BossEnemy::FixCollision(BoxCollider* hitEnemyBox, BoxCollider* hitPlayerBox)
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

void BossEnemy::SetAttackHitBox(float scale)
{
	// �U������p�{�b�N�X�̐���
	mAttackBox = new BoxCollider(this);

	// �G�O�������̓����蔻��
	AABB box = mEnemyBox;
	box.mMin *= 1.5;
	box.mMax *= 1.5;
	mAttackBox->SetObjectBox(box);
}

//EnemyBase* BossEnemy::Clone()
//{
//	return new BossEnemy(mTarget);
//}


void BossEnemy::LoadModel()
{
	mMesh = RENDERER->GetMesh("Assets/Character/Enemy/BossEnemy/BossSpider.gpmesh");
}

void BossEnemy::LoadSkeleton()
{
	mSkelMeshComponent = new SkeletalMeshComponent(this);
	mSkelMeshComponent->SetMesh(mMesh);
	mSkelMeshComponent->SetSkeleton(RENDERER->GetSkeleton("Assets/Character/Enemy/BossEnemy/BossSpider.gpskel"));
}

void BossEnemy::LoadAnimation()
{
	// �A�j���[�V�����z��ɏ�Ԃ�ǉ�
	//mAnimations.emplace(EnemyStateEnum::Spawn, RENDERER->GetAnimation("Assets/Character/Enemy/BossEnemy/BossSpider_Spawn.gpanim", false));
	mAnimations.emplace(EnemyStateEnum::Roar, RENDERER->GetAnimation("Assets/Character/Enemy/Animation/Spider_Roar.gpanim", false));
	mAnimations.emplace(EnemyStateEnum::Idle, RENDERER->GetAnimation("Assets/Character/Enemy/Animation/Spider_Idle.gpanim", false));
	mAnimations.emplace(EnemyStateEnum::Run, RENDERER->GetAnimation("Assets/Character/Enemy/Animation/Spider_Walk.gpanim", true));
	mAnimations.emplace(EnemyStateEnum::Attack2, RENDERER->GetAnimation("Assets/Character/Enemy/Animation/Spider_Charge.gpanim", false));
	mAnimations.emplace(EnemyStateEnum::Death, RENDERER->GetAnimation("Assets/Character/Enemy/Animation/Spider_Death.gpanim", false));
}

void BossEnemy::BehaviorResister()
{
	mEnemyBehaviorComponent = new EnemyBehaviorComponent(this);
	mEnemyBehaviorComponent->RegisterState(new EnemyRoar(mEnemyBehaviorComponent));
	mEnemyBehaviorComponent->RegisterState(new EnemyIdle(mEnemyBehaviorComponent));
	mEnemyBehaviorComponent->RegisterState(new EnemyChase(mEnemyBehaviorComponent, mTarget));
	mEnemyBehaviorComponent->RegisterState(new EnemyCharge(mEnemyBehaviorComponent));
	mEnemyBehaviorComponent->RegisterState(new EnemyDeath(mEnemyBehaviorComponent));
	mEnemyBehaviorComponent->SetFirstState(EnemyStateEnum::Roar);
}

void BossEnemy::SetCollider()
{
	if (mHitBox)return;
	mEnemyBox = mMesh->GetCollisionBox();
	mEnemyBox.mMin.y *= 0.5f;
	mEnemyBox.mMax.y *= 0.5f;
	mHitBox = new BoxCollider(this);
	mHitBox->SetObjectBox(mEnemyBox);
	//mHitBox->SetArrowRotate(true);
}

void BossEnemy::RemoveCollider()
{
	if (mHitBox)
	{
		delete mHitBox;
		mHitBox = nullptr;
	}
}

void BossEnemy::SetAttackTrigger()
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
