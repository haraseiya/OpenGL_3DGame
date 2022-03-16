#include "Game.h"
#include "Renderer.h"
#include "Mesh.h"
#include "SkeletalMeshComponent.h"
#include "BoxCollider.h"
#include "Skeleton.h"
#include "Collision.h"
#include "PointLightComponent.h"

#include "EnemyBase.h"
#include "Animation.h"
#include "SkeletalMeshComponent.h"

// ��ԂɕύX���������炱������ύX
const char* enemyStateEnumName[static_cast<int>(EnemyStateEnum::StateNum)] =
{
	"EnemyStateEnum::Invalid",
	"EnemyStateEnum::Spawn",
	"EnemyStateEnum::Idle",
	"EnemyStateEnum::Walk",
	"EnemyStateEnum::Turn",
	"EnemyStateEnum::Run",
	"EnemyStateEnum::LookAround"
	"EnemyStateEnum::Attack1",
	"EnemyStateEnum::Attack2",
	"EnemyStateEnum::Attack3",
	"EnemyStateEnum::GetDamage",
	"EnemyStateEnum::Roar",
	"EnemyStateEnum::Stun",
	"EnemyStateEnum::Die",

	"EnemyStateEnum::StateNum"
};

EnemyBase::EnemyBase()
	: GameObject(Tag::ENEMY)
	, mNowState(EnemyStateEnum::Invalid)
	, mEnemyStateScene(EnemyStateScene::ENEMY_SCENE_GAME)
	, mVelocityVec(0.0f, 0.0f, 0.0f)
	, mForwardVec(1.0f, 0.0f, 0.0f)
	, mWalkSpeed(0.0f)
	, mRunSpeed(0.0f)
	, mTurnSpeed(0.0f)
	, mSkelMeshComponent(nullptr)
	, mFieldOfView(10.0f)
	, mMaxFriendsVisible(30)
	, mShootTimer(0.0f)
	, mIsBossDead(false)
{
	// �������v���C���[�����ɒ���
	RotateToNewForward(Vector3::NegUnitX);
}

// state�ɃZ�b�g����Ă���A�j���[�V�������Đ�
void EnemyBase::PlayAnimation(EnemyStateEnum state, float rate)
{
	auto itr = mAnimations.find(state);
	if (itr == mAnimations.end()) return;

	// �A�j���[�V�����������Ă���΍Đ�
	mSkelMeshComponent->PlayAnimation(mAnimations[state], rate);
}

void EnemyBase::SetForwardVec(Vector3& v)
{
	mForwardVec = v;
	RotateToNewForward(v);
}

// �A�j���[�V�����͍Đ������H
bool EnemyBase::IsAnimationPlaying()
{
	return mSkelMeshComponent->IsPlaying();
}

// TriggerBox�̃Z�b�g BGTrigger�Ƃ���BoxCollider���Z�b�g
void EnemyBase::SetTriggerBox(EnemyTriggerEnum trigerType, AABB& box)
{
	BoxCollider* tb = new BoxCollider(this);
	tb->SetObjectBox(box);

	// trigerType���L�[�Ƃ��� TrigerBox���Z�b�g
	mTrigerBoxs.emplace(trigerType, tb);
}

// TrigerBox���Z�b�g����Ă���΁A�g���K�[�{�b�N�X�̃q�b�g�̗L����Ԃ�
//bool EnemyBase::IsHitTrigger(EnemyTriggerEnum type)
//{
//	auto itr = mTrigerBoxs.find(type);
//	if (itr == mTrigerBoxs.end())
//	{
//		return false;
//	}
//	return itr->second->IsTrigerHit();
//}

// TriggerEnum��TrigerBox�͓o�^����Ă��邩�H
bool EnemyBase::IsExistTriggerBox(EnemyTriggerEnum type)
{
	auto iter = mTrigerBoxs.find(type);
	return iter != mTrigerBoxs.end();
}

const char* EnemyBase::GetEnemyStateEnumName(EnemyStateEnum state)
{
	return enemyStateEnumName[static_cast<int>(state)];
}

//Vector3 EnemyBase::KeepDistance()
//{
//	return Vector3();
//}
//
//Vector3 EnemyBase::MatchHeading()
//{
//
//	return Vector3();
//}
//
//Vector3 EnemyBase::SteerToCenter()
//{
//
//	return Vector3();
//}
//
//float EnemyBase::CanISee(EnemyBase* enemy)
//{
//	if (this == enemy) return;
//
//	Vector3 d = mPosition - enemy->GetPosition();
//
//	const bool canSee = mFieldOfView > d.Length();
//	if (canSee)
//	{
//		return d.Length();
//	}
//}
//
//int EnemyBase::SeeFriends(EnemyBase* firstBoid)
//{
//	for (EnemyBase* flockMate = firstBoid; flockMate != nullptr; flockMate = flockMate->getnext())
//	{
//		float d;
//		if (d=CanISee(flockMate)) != )
//		{
//
//		}
//	}
//	return 0;
//}
//
//void EnemyBase::ClearVisibleList()
//{
//	for (int i = 0; i < mMaxFriendsVisible; i++)
//	{
//
//	}
//}
//
//void EnemyBase::AddToVisibleList(EnemyBase* enemy)
//{
//}


// �A�j���[�V�����̏�����
void EnemyBase::LoadAnimation(std::string& animationFileName, bool loop, EnemyStateEnum state)
{
	const Animation* anim = RENDERER->GetAnimation(animationFileName.c_str(), loop);

	// �A�z�z��� state���L�[�Ƃ��ăA�j���[�V������ǉ�
	mAnimations.emplace(state, anim);
}

// ���b�V���f�[�^�̓ǂݍ���0
void EnemyBase::LoadSkeletalMesh(std::string& gpmeshFileName, std::string& gskelFilename)
{
	Mesh* mesh = RENDERER->GetMesh(gpmeshFileName.c_str());
	mSkelMeshComponent = new SkeletalMeshComponent(this);
	mSkelMeshComponent->SetMesh(mesh);
	mSkelMeshComponent->SetSkeleton(RENDERER->GetSkeleton(gskelFilename.c_str()));
}

