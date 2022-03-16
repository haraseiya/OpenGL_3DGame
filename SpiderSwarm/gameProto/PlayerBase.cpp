#include "PlayerBase.h"
#include "Game.h"
#include "Input.h"
#include "Mesh.h"
#include "Renderer.h"
#include "Skeleton.h"
#include "Animation.h"
#include "PhysicsWorld.h"
#include "Collision.h"
#include "ChantEffect.h"
#include "PlayerBullet.h"
#include "SceneBase.h"

// �R���|�[�l���g�֘A
#include "BoxCollider.h"
#include "MeshComponent.h"
#include "SkeletalMeshComponent.h"
#include "InvincibleComponent.h"
#include "AttachMeshComponent.h"
#include "EffectComponent.h"

// �A�j���[�V�����X�e�[�g�֘A
#include "PlayerStateBase.h"
#include "PlayerStateRunForward.h"
#include "PlayerStateIdle.h"

const float PlayerBase::mAnimationSpeed = 0.5f;			// �A�j���[�V�����X�s�[�h
const float PlayerBase::mInterval = 0.3f;				// 
const float PlayerBase::mInvincibleTime = 3.0f;			// 
const float PlayerBase::mSpecialShotInterval = 5.0f;
const float PlayerBase::mBulletSpeed = 1000.0f;
const int PlayerBase::mRequireExperience = 10;
const float PlayerBase::mBulletScale = 1.0f;

// ��ԂɕύX���������炱������ύX
const char* playerStateEnumName[static_cast<int>(PlayerStateEnum::StateNum)] =
{
	"PlayerStateEnum::Invalid",
	"PlayerStateEnum::Spawn",
	"PlayerStateEnum::Idle",
	"PlayerStateEnum::Walk",
	"PlayerStateEnum::Turn",
	"PlayerStateEnum::Run",
	"PlayerStateEnum::LookAround"
	"PlayerStateEnum::Attack1",
	"PlayerStateEnum::Attack2",
	"PlayerStateEnum::Attack3",
	"PlayerStateEnum::GetDamage",
	"PlayerStateEnum::Roar",
	"PlayerStateEnum::Stun",
	"PlayerStateEnum::Die",

	"PlayerStateEnum::StateNum"
};

PlayerBase::PlayerBase()
	: GameObject(Tag::PLAYER)
	, mShootTimer(0.0f)
	, mIsDeadAnimFinish(false)
	, mPlayerSceneState(PlayerSceneState::PLAYER_TITLESCENE)
	, mLevel(1)
	, mExperience(0)
	, mInvincibleTimer(0.0f)
	, mHitPoint(10)
	, mInvincibleComp(nullptr)
{	
	// �傫����100����1��
	mScale = 0.01f;
	printf("PlayerActor�쐬 id:[%5d] this : (0x%p)\n", mID, this);
	mInvincibleComp = new InvincibleComponent(this);
}

PlayerBase::~PlayerBase()
{
	mAnimTypes.clear(); //�A�j���[�V�����{�̂̏����̓����_���[���ōs����
	printf("PlayerActor�j�� id:[%5d] this : (0x%p)\n", mID, this);
}

void PlayerBase::UpdateActor(float deltaTime)
{
	// �̗͂�0�ɒ���
	if (mHitPoint < 0)
	{
		mHitPoint = 0;
	}

	mMeshComp->SetVisible(false);
}

SkeletalMeshComponent* PlayerBase::GetSkeletalMeshComp()
{
	return mMeshComp;
}

const Animation* PlayerBase::GetAnim(PlayerState state)
{
	return mAnimTypes[static_cast<unsigned int>(state)];
}

// �v���C���[�Փ˔���
void PlayerBase::OnCollisionEnter(ColliderComponent* own, ColliderComponent* other)
{
	//// ���������I�u�W�F�N�g�̃^�O�擾
	//Tag colliderTag = other->GetTag();

	//// �e���q�b�g������
	//const bool isHitBullet = colliderTag == Tag::ENEMY_BULLET;
	//const bool isInvicible = mInvincibleTimer > mInvincibleTime;
	//const bool isDash = mPlayerBehavior->GetPlayerState() == PlayerStateEnum::RunForward;

	//// �G�̒e�ɓ���������v���C���[�̗̑͂��P���炷
	//if (isHitBullet && isInvicible||isDash)
	//{
	//	mInvincibleTimer = 0.0f;
	//	
	//	//mMeshComp->SetVisible(false);

	//	mHitPoint--;
	//}

	//// �Փ˂����̂��w�i�̏ꍇ
	//if (colliderTag == Tag::BACK_GROUND)
	//{
	//	if (other->GetColliderType() == ColliderTypeEnum::Wall)
	//	{
	//		CollisionInfo info = mHitBox->GetCollisionInfo();
	//		mPosition += info.mFixVec;

	//		// �ʒu���ς�����̂Ń{�b�N�X�Čv�Z
	//		ComputeWorldTransform();
	//	}
	//}

	//// �Փ˂������̂̃^�O���G�̏ꍇ
	//if (colliderTag == Tag::ENEMY)
	//{
	//	// �ʂ̃^�C�v���{�b�N�X�̏ꍇ
	//	if (other->GetColliderType() == ColliderTypeEnum::Box)
	//	{
	//		Vector3 fix;

	//		// �ǂƂԂ������Ƃ�
	//		AABB playerBox = mHitBox->GetWorldBox();
	//		AABB enemyBox = dynamic_cast<BoxCollider*>(other)->GetWorldBox();

	//		// �߂荞�݂��C��
	//		calcCollisionFixVec(playerBox, enemyBox, fix);

	//		// �␳�x�N�g�����߂�
	//		mPosition = Vector3::Lerp(mPosition,mPosition+fix,0.1);
	//		mPosition.z = 500.0f;

	//		// �ʒu���ς�����̂Ń{�b�N�X�Čv�Z
	//		ComputeWorldTransform();
	//	}
	//}

	//// �A�C�e���ɏՓ˂�����o���l�ǉ�
	//if (colliderTag == Tag::ITEM)
	//{
	//	mExperience++;
	//}
}

// �v���C���[�̃X�e�[�g�����擾
const char* PlayerBase::GetPlayerStateEnumName(PlayerStateEnum state)
{
	return playerStateEnumName[static_cast<int>(state)];
}

void PlayerBase::SetForwardVec(Vector3& v)
{
	mForwardVec = v;
	RotateToNewForward(v);
}

void PlayerBase::SetVisivle(bool isVisible)
{
	mMeshComp->SetVisible(isVisible);
}
