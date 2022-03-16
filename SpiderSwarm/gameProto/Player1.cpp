#include "Player1.h"
#include "Game.h"
#include "Input.h"
#include "Mesh.h"
#include "Renderer.h"
#include "MeshComponent.h"
#include "SkeletalMeshComponent.h"
#include "Skeleton.h"
#include "Animation.h"
#include "PhysicsWorld.h"
#include "BoxCollider.h"
#include "Collision.h"
#include "AttachMeshComponent.h"
#include "EffectComponent.h"
#include "ChantEffect.h"
#include "PlayerBullet.h"
#include "BulletManager.h"
#include "LaserEffect.h"
#include "AttachMeshComponent.h"
#include "InvincibleComponent.h"
//#include "Weapon.h"

// �v���C���[�X�e�[�g�֘A
#include "PlayerBehaviorComponent.h"
#include "PlayerStateBase.h"
#include "PlayerStateRunForward.h"
#include "PlayerStateIdle.h"
#include "PlayerStateDie.h"
#include "PlayerStateHold.h"
#include "PlayerStateForward.h"
#include "PlayerStateRunForward.h"
#include "PlayerStateVictory.h"

const int Player1::mLevelLimit = 5;

Player1::Player1()
	: mNowState(PlayerState::PLAYER_STATE_IDLE)
	, mNextState(PlayerState::PLAYER_STATE_IDLE)
	, mBullet(nullptr)
{
	printf("�v���C���[�P�쐬\n");

	mLevel = 1;				// ���x��
	mHitPoint = 10;		// �̗�

	// �v���C���[�ӂ�܂��N���X�̐���
	mPlayerBehavior = new PlayerBehaviorComponent(this);

	// ���\�[�X�̓ǂݍ���
	LoadModel();
	LoadSkeleton();
	LoadAnimation();
	//AttachWeapon();

	// �ӂ�܂���ǉ�
	BehaviorResister();

	// �����蔻��Z�b�g
	SetCollider();
}

Player1::~Player1()
{
	//�A�j���[�V�����{�̂̏����̓����_���[���ōs����
	mAnimTypes.clear(); 
	printf("PlayerActor�j�� id:[%5d] this : (0x%p)\n", mID, this);
}

void Player1::UpdateActor(float deltaTime)
{
	// �̗͂��O�ɂȂ����ꍇ
	if (mHitPoint<=0)
	{
		SetVisivle(true);
	}

	// ���x���A�b�v�ɕK�v�Ȍo���l���W�߂��烌�x���A�b�v
	const bool isLevelUp = mExperience >= mRequireExperience;
	if (isLevelUp)
	{
		// ���x���������𒴂�����K������
		if (mLevel>mLevelLimit)
		{
			mLevel = mLevelLimit;
		}

		// �o���l���O�ɖ߂��ă��x�����P�グ��
		mExperience = 0;
		mLevel++;
	}

	//// �X�y�V�����V���b�g�����Ă�Ȃ�
	//mSpecialShotTimer += deltaTime;
	//const bool isSpecialShot= INPUT_INSTANCE.IsKeyPressed(KEY_Y) && mSpecialShotTimer > mSpecialShotInterval;
	//if (isSpecialShot)
	//{
	//	mSpecialShotTimer = 0.0f;
	//	mLaser = new LaserEffect(this);
	//}

	mInvincibleTimer += deltaTime;
}

SkeletalMeshComponent* Player1::GetSkeletalMeshComp()
{
	return mMeshComp;
}

const Animation* Player1::GetAnim(PlayerState state)
{
	return mAnimTypes[static_cast<unsigned int>(state)];
}

// ���f���̃��[�h
void Player1::LoadModel()
{
	//���b�V���̃��[�h
	mMesh = RENDERER->GetMesh("Assets/Character/Player1/Player1.gpmesh");
	mMeshComp = new SkeletalMeshComponent(this);
	mMeshComp->SetMesh(mMesh);
}

// �X�P���g���̃��[�h
void Player1::LoadSkeleton()
{
	// �X�P���g��
	mMeshComp->SetSkeleton(RENDERER->GetSkeleton("Assets/Character/Player1/Player1.gpskel"));
}

// �A�j���[�V�����̃��[�h
void Player1::LoadAnimation()
{
	// �A�j���[�V�����������̃��������m��
	mAnimTypes.resize(static_cast<unsigned int>(PlayerState::PLAYER_STATE_NUM));

	// �V�[�����̃A�j���[�V�����ǂݍ���
	switch (mPlayerSceneState)
	{
	// �^�C�g���V�[��
	case PlayerSceneState::PLAYER_TITLESCENE:
		mAnimTypes[static_cast<unsigned int>(PlayerState::PLAYER_STATE_HOLD)] = RENDERER->GetAnimation("Assets/Character/Player1/Animation/Player1_Hold.gpanim", false);
		mPlayerBehavior->RegisterState(new PlayerStateHold(mPlayerBehavior));
		mPlayerBehavior->SetFirstState(PlayerStateEnum::Hold);
		break;  

	// �Z���N�g�V�[��
	case PlayerSceneState::PLAYER_SELECTSCENE:
		// �I�𒆂̃A�j���[�V����

		// �I����̃A�j���[�V����

		break;

	// �Q�[���V�[��
	case PlayerSceneState::PLAYER_GAMESCENE:
		mAnimTypes[static_cast<unsigned int>(PlayerState::PLAYER_STATE_IDLE)] = RENDERER->GetAnimation("Assets/Character/Player1/Animation/Player1_Idle.gpanim", true);
		mAnimTypes[static_cast<unsigned int>(PlayerState::PLAYER_STATE_FORWARD)] = RENDERER->GetAnimation("Assets/Character/Player1/Animation/Player1_Forward.gpanim", true);
		// �c��3�������̃A�j���[�V����

		// 4�������̑���A�j���[�V����
		mAnimTypes[static_cast<unsigned int>(PlayerState::PLAYER_STATE_RUN_FORWARD)] = RENDERER->GetAnimation("Assets/Character/Player1/Animation/Player1_Sprint_Forward.gpanim", true);

		mAnimTypes[static_cast<unsigned int>(PlayerState::PLAYER_STATE_DIE)] = RENDERER->GetAnimation("Assets/Character/Player1/Animation/Player1_Death.gpanim", false);
		mPlayerBehavior->RegisterState(new PlayerStateIdle(mPlayerBehavior));
		mPlayerBehavior->RegisterState(new PlayerStateForward(mPlayerBehavior));
		mPlayerBehavior->RegisterState(new PlayerStateRunForward(mPlayerBehavior));
		mPlayerBehavior->RegisterState(new PlayerStateDie(mPlayerBehavior));
		mPlayerBehavior->SetFirstState(PlayerStateEnum::Idle);
		break;

	// ���U���g�V�[��
	case PlayerSceneState::PLAYER_RESULT:
		// �����A�j���[�V����
		mAnimTypes[static_cast<unsigned int>(PlayerState::PLAYER_STATE_VICTORY)] = RENDERER->GetAnimation("Assets/Character/Player1/Animation/Player1_Victory.gpanim", false);
		mPlayerBehavior->RegisterState(new PlayerStateVictory(mPlayerBehavior));
		mPlayerBehavior->SetFirstState(PlayerStateEnum::Victory);
		break;
	}
}

// �ӂ�܂��̓o�^
void Player1::BehaviorResister()
{
}

// ���g�̃R���C�_�[�̃Z�b�g
void Player1::SetCollider()
{
	// �����蔻��Z�b�g
	mPlayerBox = mMesh->GetCollisionBox();
	mHitBox = new BoxCollider(this);
	//mPlayerBox.mMin.x *= 1.0f;
	//mPlayerBox.mMin.y *= 1.0f;
	//mPlayerBox.mMax.x *= 1.0f;
	//mPlayerBox.mMax.y *= 1.0f;
	mHitBox->SetObjectBox(mPlayerBox);
}

void Player1::OnCollisionEnter(ColliderComponent* own, ColliderComponent* other)
{
	// ���������I�u�W�F�N�g�̃^�O�擾
	Tag colliderTag = other->GetTag();

	// �e���q�b�g������

	const bool isHitEnemyBullet = colliderTag == Tag::ENEMY_BULLET;
	const bool isDash = mPlayerBehavior->GetPlayerState() == PlayerStateEnum::RunForward;

	// �G�̒e�ɓ���������v���C���[�̗̑͂��P���炷
	if (isHitEnemyBullet &&!mInvincibleComp->GetIsInvicible())
	{
		if (mHitPoint > 0)
		{
			mHitPoint--;
		}

		mInvincibleComp->SetIsInvicible(true);
	}

	// �Փ˂����̂��w�i�̏ꍇ
	if (colliderTag == Tag::BACK_GROUND)
	{
		if (other->GetColliderType() == ColliderTypeEnum::Wall)
		{
			CollisionInfo info = mHitBox->GetCollisionInfo();
			mPosition += info.mFixVec;

			// �ʒu���ς�����̂Ń{�b�N�X�Čv�Z
			ComputeWorldTransform();
		}
	}

	// �Փ˂������̂̃^�O���G�̏ꍇ
	if (colliderTag == Tag::ENEMY)
	{
		// �ʂ̃^�C�v���{�b�N�X�̏ꍇ
		if (other->GetColliderType() == ColliderTypeEnum::Box)
		{
			Vector3 fix;

			// �ǂƂԂ������Ƃ�
			AABB playerBox = mHitBox->GetWorldBox();
			AABB enemyBox = dynamic_cast<BoxCollider*>(other)->GetWorldBox();

			// �߂荞�݂��C��
			calcCollisionFixVec(playerBox, enemyBox, fix);

			// �␳�x�N�g�����߂�
			mPosition = Vector3::Lerp(mPosition, mPosition + fix, 0.1f);
			mPosition.z = 500.0f;

			// �ʒu���ς�����̂Ń{�b�N�X�Čv�Z
			ComputeWorldTransform();
		}
	}

	// �o���l�A�C�e���ɏՓ˂�����o���l�ǉ�
	if (colliderTag == Tag::ITEM_EXPERIENCE)
	{
		mExperience++;
	}

	// �񕜃A�C�e���ɏՓ˂�����̗͉�
	if (colliderTag == Tag::ITEM_HEAL)
	{
		mHitPoint += 2;
	}
}

//// �v���C���[�ɕ�����A�^�b�`
//void Player1::AttachWeapon()
//{
//	AttachMeshComponent* attachMesh;
//	attachMesh = new AttachMeshComponent(this, mMeshComp, "hand_l");
//	attachMesh->SetMesh(RENDERER->GetMesh("Assets/Weapon/AK-47/AK-47.gpmesh"));
//
//	Vector3 rot, pos;
//	rot = Vector3(-Math::PiOver2 * 0.5f, Math::Pi, 0);
//	pos = Vector3(0, 0, 0);
//	attachMesh->SetOffsetRotation(rot);
//	attachMesh->SetOffsetPosition(pos);
//}
