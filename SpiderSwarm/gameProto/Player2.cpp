#include "Player2.h"
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

#include "PlayerStateBase.h"
#include "PlayerStateRunForward.h"
#include "PlayerStateIdle.h"

const float cAnimationSpeed = 0.5f;
const float Player2::m_range = 10.0f;
const float Player2::mInterval = 0.1f;

Player2::Player2()
	: mNowState(PlayerState::PLAYER_STATE_IDLE)
	, mNextState(PlayerState::PLAYER_STATE_IDLE)
	, mShootTimer(0.0f)
{
	printf("�v���C���[2�쐬\n");

	LoadModel();
	LoadSkeleton();
	LoadAnimation();
	BehaviorResister();
	SetCollider();
}

Player2::~Player2()
{
	mAnimTypes.clear(); //�A�j���[�V�����{�̂̏����̓����_���[���ōs����
	printf("PlayerActor�j�� id:[%5d] this : (0x%p)\n", mID, this);
}

void Player2::UpdateActor(float deltaTime)
{
}

// �w�iAABB�Ƃ̃q�b�g�߂荞�݉��� ( ���������ۂ�PhysicsWorld����Ă΂�� �j
void Player2::FixCollision(BoxCollider* hitPlayerBox, BoxCollider* hitBox)
{
}

SkeletalMeshComponent* Player2::GetSkeletalMeshComp()
{
	return mMeshComp;
}

const Animation* Player2::GetAnim(PlayerState state)
{
	return mAnimTypes[static_cast<unsigned int>(state)];
}

void Player2::OnCollisionEnter(ColliderComponent* own, ColliderComponent* other)
{
}

void Player2::LoadModel()
{
	//���b�V���̃��[�h
	mMesh = RENDERER->GetMesh("Assets/Mesh/Player.gpmesh");
	mMeshComp = new SkeletalMeshComponent(this);
	mMeshComp->SetMesh(mMesh);
}

void Player2::LoadSkeleton()
{
	// �X�P���g��
	mMeshComp->SetSkeleton(RENDERER->GetSkeleton("Assets/Skelton/Player.gpskel"));
}

void Player2::LoadAnimation()
{
	// �A�j���[�V�����̎擾 & �A�j���[�V�����z��ɃZ�b�g
	mAnimTypes.resize(static_cast<unsigned int>(PlayerState::PLAYER_STATE_NUM));
	mAnimTypes[static_cast<unsigned int>(PlayerState::PLAYER_STATE_IDLE)] = RENDERER->GetAnimation("Assets/Animation/Player_Idle.gpanim", true);
	mAnimTypes[static_cast<unsigned int>(PlayerState::PLAYER_STATE_RUN_FORWARD)] = RENDERER->GetAnimation("Assets/Animation/Player_Running.gpanim", true);
}

void Player2::BehaviorResister()
{
	// �v���C���[�X�e�[�g�v�[���̏�����
	mPlayerBehavior = new PlayerBehaviorComponent(this);
	mPlayerBehavior->RegisterState(new PlayerStateIdle(mPlayerBehavior));
	mPlayerBehavior->RegisterState(new PlayerStateRunForward(mPlayerBehavior));
	mPlayerBehavior->SetFirstState(PlayerStateEnum::Idle);
}

void Player2::SetCollider()
{
	// �����蔻��Z�b�g
	mPlayerBox = mMesh->GetCollisionBox();
	mHitBox = new BoxCollider(this);
	mPlayerBox.mMin.x *= 1.2f;
	mPlayerBox.mMin.y *= 1.2f;
	mPlayerBox.mMax.x *= 1.2f;
	mPlayerBox.mMax.y *= 1.2f;
	mHitBox->SetObjectBox(mPlayerBox);
}
