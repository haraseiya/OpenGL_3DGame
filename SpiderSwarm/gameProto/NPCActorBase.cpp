#include "Game.h"
#include "Renderer.h"
#include "Mesh.h"
#include "SkeletalMeshComponent.h"
#include "BoxCollider.h"

#include "NPCActorBase.h"
#include "Animation.h"
#include "SkeletalMeshComponent.h"

// 状態に変更があったらこちらも変更
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

//　コンストラクタ
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

// stateにセットされているアニメーションを再生
float NPCActorBase::PlayAnimation(NPCStateEnum state, float rate)
{
	auto itr = mAnimations.find(state);
	if (itr == mAnimations.end())
	{
		return 0.0f;
	}

	// アニメーションを持っていれば再生
	return mSkelMeshComponent->PlayAnimation(mAnimations[state], rate);
}

// 前方方向をセット
void NPCActorBase::SetForwardVec(Vector3& v)
{
	mForwardVec = v;
	RotateToNewForward(v);
}

// アニメーションは再生中か？
bool NPCActorBase::IsAnimationPlaying()
{
	return mSkelMeshComponent->IsPlaying();
}

// TriggerBoxのセット BGTriggerとしてBoxColliderをセット
void NPCActorBase::SetTriggerBox(NPCTriggerEnum trigerType, AABB& box)
{
	BoxCollider* tb = new BoxCollider(this);
	tb->SetObjectBox(box);
	// trigerTypeをキーとして TrigerBoxをセット
	mTrigerBoxs.emplace(trigerType, tb);
}

// TrigerBoxがセットされていれば、トリガーボックスのヒットの有無を返す
//bool NPCActorBase::IsHitTrigger(NPCTriggerEnum type)
//{
//	auto itr = mTrigerBoxs.find(type);
//	if (itr == mTrigerBoxs.end())
//	{
//		return false;
//	}
//	return itr->second->IsTrigerHit();
//}

// TriggerEnumのTrigerBoxは登録されているか？
bool NPCActorBase::IsExistTriggerBox(NPCTriggerEnum type)
{
	auto iter = mTrigerBoxs.find(type);
	return iter != mTrigerBoxs.end();
}

void NPCActorBase::HitEnemyAttack(const int& power)
{
	mHitPoint -= power;
}

// アニメーションの初期化
void NPCActorBase::LoadAnimation(std::string& animationFileName, bool loop, NPCStateEnum state)
{
	// アニメーションの取得
	const Animation* anim = RENDERER->GetAnimation(animationFileName.c_str(), loop);

	// 連想配列に stateをキーとしてアニメーションを追加
	mAnimations.emplace(state, anim);
}

// メッシュデータの読み込み
void NPCActorBase::LoadSkeletalMesh(std::string& gpmeshFileName, std::string& gskelFilename)
{
	Mesh* mesh = RENDERER->GetMesh(gpmeshFileName.c_str());
	mSkelMeshComponent = new SkeletalMeshComponent(this);
	mSkelMeshComponent->SetMesh(mesh);
	mSkelMeshComponent->SetSkeleton(RENDERER->GetSkeleton(gskelFilename.c_str()));
}

// 状態名を文字列で返す
const char* GetNPCStateEnumName(NPCStateEnum state)
{
	return NPCStateEnumName[static_cast<int>(state)];
}
