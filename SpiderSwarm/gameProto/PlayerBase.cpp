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

// コンポーネント関連
#include "BoxCollider.h"
#include "MeshComponent.h"
#include "SkeletalMeshComponent.h"
#include "InvincibleComponent.h"
#include "AttachMeshComponent.h"
#include "EffectComponent.h"

// アニメーションステート関連
#include "PlayerStateBase.h"
#include "PlayerStateRunForward.h"
#include "PlayerStateIdle.h"

const float PlayerBase::mAnimationSpeed = 0.5f;			// アニメーションスピード
const float PlayerBase::mInterval = 0.3f;				// 
const float PlayerBase::mInvincibleTime = 3.0f;			// 
const float PlayerBase::mSpecialShotInterval = 5.0f;
const float PlayerBase::mBulletSpeed = 1000.0f;
const int PlayerBase::mRequireExperience = 10;
const float PlayerBase::mBulletScale = 1.0f;

// 状態に変更があったらこちらも変更
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
	// 大きさを100分の1に
	mScale = 0.01f;
	printf("PlayerActor作成 id:[%5d] this : (0x%p)\n", mID, this);
	mInvincibleComp = new InvincibleComponent(this);
}

PlayerBase::~PlayerBase()
{
	mAnimTypes.clear(); //アニメーション本体の消去はレンダラー側で行われる
	printf("PlayerActor破棄 id:[%5d] this : (0x%p)\n", mID, this);
}

void PlayerBase::UpdateActor(float deltaTime)
{
	// 体力を0に調整
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

// プレイヤー衝突判定
void PlayerBase::OnCollisionEnter(ColliderComponent* own, ColliderComponent* other)
{
	//// 当たったオブジェクトのタグ取得
	//Tag colliderTag = other->GetTag();

	//// 弾がヒットしたら
	//const bool isHitBullet = colliderTag == Tag::ENEMY_BULLET;
	//const bool isInvicible = mInvincibleTimer > mInvincibleTime;
	//const bool isDash = mPlayerBehavior->GetPlayerState() == PlayerStateEnum::RunForward;

	//// 敵の弾に当たったらプレイヤーの体力を１減らす
	//if (isHitBullet && isInvicible||isDash)
	//{
	//	mInvincibleTimer = 0.0f;
	//	
	//	//mMeshComp->SetVisible(false);

	//	mHitPoint--;
	//}

	//// 衝突したのが背景の場合
	//if (colliderTag == Tag::BACK_GROUND)
	//{
	//	if (other->GetColliderType() == ColliderTypeEnum::Wall)
	//	{
	//		CollisionInfo info = mHitBox->GetCollisionInfo();
	//		mPosition += info.mFixVec;

	//		// 位置が変わったのでボックス再計算
	//		ComputeWorldTransform();
	//	}
	//}

	//// 衝突した物体のタグが敵の場合
	//if (colliderTag == Tag::ENEMY)
	//{
	//	// 別のタイプがボックスの場合
	//	if (other->GetColliderType() == ColliderTypeEnum::Box)
	//	{
	//		Vector3 fix;

	//		// 壁とぶつかったとき
	//		AABB playerBox = mHitBox->GetWorldBox();
	//		AABB enemyBox = dynamic_cast<BoxCollider*>(other)->GetWorldBox();

	//		// めり込みを修正
	//		calcCollisionFixVec(playerBox, enemyBox, fix);

	//		// 補正ベクトル分戻す
	//		mPosition = Vector3::Lerp(mPosition,mPosition+fix,0.1);
	//		mPosition.z = 500.0f;

	//		// 位置が変わったのでボックス再計算
	//		ComputeWorldTransform();
	//	}
	//}

	//// アイテムに衝突したら経験値追加
	//if (colliderTag == Tag::ITEM)
	//{
	//	mExperience++;
	//}
}

// プレイヤーのステート名を取得
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
