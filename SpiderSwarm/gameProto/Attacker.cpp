#include "Attacker.h"
#include "Game.h"
#include "Renderer.h"
#include "Mesh.h"
#include "Skeleton.h"
#include "SkeletalMeshComponent.h"
#include "AttachMeshComponent.h"
#include "Collision.h"
#include "BoxCollider.h"
#include "Input.h"
#include "EffectComponent.h"

#include "NPCBehaviorComponent.h"
#include "NPCIdle.h"
#include "NPCDie.h"
#include "NPCRun.h"
#include "NPCPatrol.h"
#include "NPCLookAround.h"
#include "NPCAttack.h"

#include <iostream>

const int Attacker::m_maxHP = 100;
const float Attacker::m_maxSpeed = 500.0f;

Attacker::Attacker(Player* player,EnemyBase* enemy)
	: mNPCBehaviorComponent(nullptr)
	, mAttackTriggerBox(nullptr)
	, mHitBox(nullptr)
	, mCoolTime(0.0f)
{
	printf("Create : [Actor] Attacker 0x%p\n", this);

	// ‰Šúƒpƒ‰ƒ[ƒ^Ý’è
	mWalkSpeed = 100.0f;
	mRunSpeed = m_maxSpeed;
	mTurnSpeed = Math::Pi;
	mHitPoint = m_maxHP;
	mIsOnGround = true;
	mScale = 0.01f;

	// ƒ‚ƒfƒ‹“Ç‚Ýž‚Ý
	mSkelMeshComponent = new SkeletalMeshComponent(this);
	Mesh* mesh = RENDERER->GetMesh("Assets/Mesh/Attacker1.gpmesh");
	mSkelMeshComponent->SetMesh(mesh);
	mSkelMeshComponent->SetSkeleton(RENDERER->GetSkeleton("Assets/Skelton/Attacker1.gpskel"));

	// ƒAƒjƒ[ƒVƒ‡ƒ““Ç‚Ýž‚Ý
	mAnimations.emplace(NPCStateEnum::Idle, RENDERER->GetAnimation("Assets/Animation/NPCIdle_Anim.gpanim", true));
	mAnimations.emplace(NPCStateEnum::Walk, RENDERER->GetAnimation("Assets/Animation/Attacker_Walking.gpanim", true));
	mAnimations.emplace(NPCStateEnum::Run, RENDERER->GetAnimation("Assets/Animation/NPCRun.gpanim", true));
	mAnimations.emplace(NPCStateEnum::Attack1, RENDERER->GetAnimation("Assets/Animation/NPCDeath.gpanim", false));
	mAnimations.emplace(NPCStateEnum::Die, RENDERER->GetAnimation("Assets/Animation/NPCDeath.gpanim", false));

	//// EnemyBehaviorComponent ‚É ‚Ó‚é‚Ü‚¢‚ð’Ç‰Á
	mNPCBehaviorComponent = new NPCBehaviorComponent(this);
	mNPCBehaviorComponent->RegisterState(new NPCIdle(mNPCBehaviorComponent,player,enemy));
	mNPCBehaviorComponent->RegisterState(new NPCPatrol(mNPCBehaviorComponent));
	mNPCBehaviorComponent->RegisterState(new NPCLookAround(mNPCBehaviorComponent));
	mNPCBehaviorComponent->RegisterState(new NPCRun(mNPCBehaviorComponent,player,enemy));
	mNPCBehaviorComponent->RegisterState(new NPCAttack(mNPCBehaviorComponent, enemy));
	mNPCBehaviorComponent->RegisterState(new NPCDie(mNPCBehaviorComponent));
	mNPCBehaviorComponent->SetFirstState(NPCStateEnum::Run);

	// NPC‚Ì“–‚½‚è”»’è‚ð’Ç‰Á
	AABB npcBox = mesh->GetCollisionBox();
	npcBox.mMin.x *= 0.5f;
	npcBox.mMax.x *= 0.5f;
	mHitBox = new BoxCollider(this);
	mHitBox->SetObjectBox(npcBox);
	mHitBox->SetArrowRotate(false);

	// NPC‚Ì‘O•ûƒgƒŠƒK[‚ð’Ç‰Á
	//AABB npcForward;
	//npcForward.mMin.x = npcBox.mMax.x;
	//npcForward.mMin.y = npcBox.mMin.y;
	//npcForward.mMin.z = npcBox.mMin.z + 100;
	//npcForward.mMax.x = npcForward.mMin.x + 100.0f;
	//npcForward.mMax.y = npcForward.mMin.y + 100.0f;
	//npcForward.mMax.z = npcForward.mMin.z + 100.0f;
	//mAttackTriggerBox = new BoxCollider(this);
	//mAttackTriggerBox->SetObjectBox(npcForward);
}

Attacker::~Attacker()
{
	printf("Remove : [Actor] BlackKnight 0x%p\n", this);
}

void Attacker::UpdateActor(float deltaTime)
{
	// ‘Ì—Í‚ª‚È‚­‚È‚Á‚½‚ç
	if (mHitPoint <= 0)
	{
		printf("NPCŽ€–S");
	}

	mCoolTime += deltaTime;

	//printf("%f\n", mCoolTime);
}

void Attacker::OnCollisionEnter(ColliderComponent* other)
{
	Tag colliderTag = other->GetTag();

	// Õ“Ë‚µ‚½‚Ì‚ª‘¼‚ÌNPC‚Ìê‡
	if (colliderTag == Tag::NPC)
	{
		if (other->GetColliderType() == ColliderTypeEnum::Box)
		{
			Vector3 fixVec;
			AABB npcBox = mHitBox->GetWorldBox();
			AABB otherNPCBox = dynamic_cast<BoxCollider*>(other)->GetWorldBox();

			calcCollisionFixVec(npcBox, otherNPCBox, fixVec);
			mPosition += fixVec;
			ComputeWorldTransform();
		}
	}

	//// “–‚½‚è”»’è‚Å‹A‚Á‚Ä‚«‚½Œ‹‰Ê‚ªmHitBoxA”wŒi‚Æ‚ÌÕ“Ë‚¾‚Á‚½ê‡
	//if (mHitBox == hitThisBox &&
	//	hitOtherBox->GetTag() == Tag::BackGround)
	//{
	//	AABB bgBox = hitOtherBox->GetWorldBox();
	//	AABB thisBox = hitThisBox->GetWorldBox();
	//	Vector3 fixVec;

	//	calcCollisionFixVec(thisBox, bgBox, fixVec);
	//	mPosition += fixVec;
	//	mHitBox->OnUpdateWorldTransform();
	//}

	//// •Ê‚ÌNPC‚Æ‚Ì“–‚½‚è”»’è

	//// “–‚½‚è”»’è‚Å‹A‚Á‚Ä‚«‚½Œ‹‰Ê‚ªŽ©g‚Æ“G‚Æ‚ÌÕ“Ë‚¾‚Á‚½ê‡
	//if (mHitBox == hitThisBox &&
	//	hitOtherBox->GetTag() == Tag::Enemy)
	//{
	//	AABB enemyBox = hitOtherBox->GetWorldBox();
	//	AABB thisBox = hitThisBox->GetWorldBox();
	//	Vector3 fixVec;

	//	calcCollisionFixVec(thisBox, enemyBox, fixVec);
	//	mPosition += fixVec;
	//	mHitBox->OnUpdateWorldTransform();
	//}

	//// ƒqƒbƒg‚µ‚½‚Ì‚ªƒvƒŒƒCƒ„[‚Ìê‡
	//if (mHitBox == hitThisBox &&
	//	hitOtherBox->GetTag() == Tag::Player)
	//{
	//	AABB playerBox = hitOtherBox->GetWorldBox();
	//	AABB thisBox = hitThisBox->GetWorldBox();
	//	Vector3 fixVec;

	//	calcCollisionFixVec(thisBox, playerBox, fixVec);
	//	mPosition += fixVec;
	//	mHitBox->OnUpdateWorldTransform();
	//}

	//if (INPUT_INSTANCE.GetInput(KEY_A) == KEY_STATE_PUSHDOWN)
	//{
	//	mCoolTime = 0.0f;
	//}

	//// ‰r¥‚Å‚«‚é‚©H
	//const bool isChant = mHitBox == hitThisBox &&
	//	//hitOtherBox->GetTag() == EnumPhysicsType::EnumPlayerTrigger &&
	//	INPUT_INSTANCE.GetInput(KEY_A) == KEY_STATE_PRESSED;

	//if (isChant)
	//{
	//	// ƒgƒŠƒK[‚É3•b“ü‚Á‚Ä‚¢‚½‚ç
	//	if (mCoolTime >= 3.0f)
	//	{
	//		printf("NPC•œŠˆ\n");
	//		mNPCBehaviorComponent->ChangeState(NPCStateEnum::Run);
	//		mCoolTime = 0.0f;
	//	}
	//}

	// “G‚ÌUŒ‚‚ªƒqƒbƒg‚µ‚½ê‡
	//if (mHitBox == hitThisBox &&
	//	hitOtherBox->GetType() == EnumPhysicsType::EnumEnemyAttackBox)
	//{
	//	mHitPoint -= 100;
	//}
}

void Attacker::GetDamage(const int& power)
{
	// ‘Ì—Í‚ðŒ¸‚ç‚·
	mHitPoint -= power;
}

//bool Attacker::IsFrontHit()
//{
//	return mAttackTriggerBox->IsTrigerHit();
//}
