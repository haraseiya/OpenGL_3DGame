#include "Game.h"
#include "Renderer.h"
#include "Mesh.h"
#include "SkeletalMeshComponent.h"
#include "BoxCollider.h"

#include "NPCActorBase.h"
#include "Animation.h"
#include "SkeletalMeshComponent.h"

// ��ԂɕύX���������炱������ύX
const char* NPCStateEnumName[static_cast<int>(NPCStateEnum::StateNum)] =
{
	"NPCStateEnum::Invalid",
	"NPCStateEnum::Spawn",
	"NPCStateEnum::Idle",
	"NPCStateEnum::Walk",
	"NPCStateEnum::Turn",
	"NPCStateEnum::Run",
	"NPCStateEnum::LookAround"
	"NPCStateEnum::Attack1",
	"NPCStateEnum::Attack2",
	"NPCStateEnum::Attack3",
	"NPCStateEnum::GetDamage",
	"NPCStateEnum::Roar",
	"NPCStateEnum::Stun",
	"NPCStateEnum::Die",

	"NPCStateEnum::StateNum"
};

//�@�R���X�g���N�^
NPCActorBase::NPCActorBase()
	: GameObject(Tag::NPC)
	, mVelocityVec(0.0f, 0.0f, 0.0f)
	, mForwardVec(1.0f, 0.0f, 0.0f)
	, mWalkSpeed(0.0f)
	, mRunSpeed(0.0f)
	, mTurnSpeed(0.0f)
	, mHitPoint(100)
	, mIsOnGround(false)
	, mSkelMeshComponent(nullptr)
{
}

// state�ɃZ�b�g����Ă���A�j���[�V�������Đ�
float NPCActorBase::PlayAnimation(NPCStateEnum state, float rate)
{
	auto itr = mAnimations.find(state);
	if (itr == mAnimations.end())
	{
		return 0.0f;
	}

	// �A�j���[�V�����������Ă���΍Đ�
	return mSkelMeshComponent->PlayAnimation(mAnimations[state], rate);
}

// �O���������Z�b�g
void NPCActorBase::SetForwardVec(Vector3& v)
{
	mForwardVec = v;
	RotateToNewForward(v);
}

// �A�j���[�V�����͍Đ������H
bool NPCActorBase::IsAnimationPlaying()
{
	return mSkelMeshComponent->IsPlaying();
}

// TriggerBox�̃Z�b�g BGTrigger�Ƃ���BoxCollider���Z�b�g
void NPCActorBase::SetTriggerBox(NPCTriggerEnum trigerType, AABB& box)
{
	BoxCollider* tb = new BoxCollider(this);
	tb->SetObjectBox(box);
	// trigerType���L�[�Ƃ��� TrigerBox���Z�b�g
	mTrigerBoxs.emplace(trigerType, tb);
}

// TrigerBox���Z�b�g����Ă���΁A�g���K�[�{�b�N�X�̃q�b�g�̗L����Ԃ�
//bool NPCActorBase::IsHitTrigger(NPCTriggerEnum type)
//{
//	auto itr = mTrigerBoxs.find(type);
//	if (itr == mTrigerBoxs.end())
//	{
//		return false;
//	}
//	return itr->second->IsTrigerHit();
//}

// TriggerEnum��TrigerBox�͓o�^����Ă��邩�H
bool NPCActorBase::IsExistTriggerBox(NPCTriggerEnum type)
{
	auto iter = mTrigerBoxs.find(type);
	return iter != mTrigerBoxs.end();
}

void NPCActorBase::HitEnemyAttack(const int& power)
{
	mHitPoint -= power;
}

// �A�j���[�V�����̏�����
void NPCActorBase::LoadAnimation(std::string& animationFileName, bool loop, NPCStateEnum state)
{
	// �A�j���[�V�����̎擾
	const Animation* anim = RENDERER->GetAnimation(animationFileName.c_str(), loop);

	// �A�z�z��� state���L�[�Ƃ��ăA�j���[�V������ǉ�
	mAnimations.emplace(state, anim);
}

// ���b�V���f�[�^�̓ǂݍ���
void NPCActorBase::LoadSkeletalMesh(std::string& gpmeshFileName, std::string& gskelFilename)
{
	Mesh* mesh = RENDERER->GetMesh(gpmeshFileName.c_str());
	mSkelMeshComponent = new SkeletalMeshComponent(this);
	mSkelMeshComponent->SetMesh(mesh);
	mSkelMeshComponent->SetSkeleton(RENDERER->GetSkeleton(gskelFilename.c_str()));
}

// ��Ԗ��𕶎���ŕԂ�
const char* GetNPCStateEnumName(NPCStateEnum state)
{
	return NPCStateEnumName[static_cast<int>(state)];
}
