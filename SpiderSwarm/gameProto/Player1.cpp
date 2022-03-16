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

// プレイヤーステート関連
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
	printf("プレイヤー１作成\n");

	mLevel = 1;				// レベル
	mHitPoint = 10;		// 体力

	// プレイヤーふるまいクラスの生成
	mPlayerBehavior = new PlayerBehaviorComponent(this);

	// リソースの読み込み
	LoadModel();
	LoadSkeleton();
	LoadAnimation();
	//AttachWeapon();

	// ふるまいを追加
	BehaviorResister();

	// あたり判定セット
	SetCollider();
}

Player1::~Player1()
{
	//アニメーション本体の消去はレンダラー側で行われる
	mAnimTypes.clear(); 
	printf("PlayerActor破棄 id:[%5d] this : (0x%p)\n", mID, this);
}

void Player1::UpdateActor(float deltaTime)
{
	// 体力が０になった場合
	if (mHitPoint<=0)
	{
		SetVisivle(true);
	}

	// レベルアップに必要な経験値を集めたらレベルアップ
	const bool isLevelUp = mExperience >= mRequireExperience;
	if (isLevelUp)
	{
		// レベルが制限を超えたら規制する
		if (mLevel>mLevelLimit)
		{
			mLevel = mLevelLimit;
		}

		// 経験値を０に戻してレベルを１上げる
		mExperience = 0;
		mLevel++;
	}

	//// スペシャルショットが撃てるなら
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

// モデルのロード
void Player1::LoadModel()
{
	//メッシュのロード
	mMesh = RENDERER->GetMesh("Assets/Character/Player1/Player1.gpmesh");
	mMeshComp = new SkeletalMeshComponent(this);
	mMeshComp->SetMesh(mMesh);
}

// スケルトンのロード
void Player1::LoadSkeleton()
{
	// スケルトン
	mMeshComp->SetSkeleton(RENDERER->GetSkeleton("Assets/Character/Player1/Player1.gpskel"));
}

// アニメーションのロード
void Player1::LoadAnimation()
{
	// アニメーション総数分のメモリを確保
	mAnimTypes.resize(static_cast<unsigned int>(PlayerState::PLAYER_STATE_NUM));

	// シーン毎のアニメーション読み込み
	switch (mPlayerSceneState)
	{
	// タイトルシーン
	case PlayerSceneState::PLAYER_TITLESCENE:
		mAnimTypes[static_cast<unsigned int>(PlayerState::PLAYER_STATE_HOLD)] = RENDERER->GetAnimation("Assets/Character/Player1/Animation/Player1_Hold.gpanim", false);
		mPlayerBehavior->RegisterState(new PlayerStateHold(mPlayerBehavior));
		mPlayerBehavior->SetFirstState(PlayerStateEnum::Hold);
		break;  

	// セレクトシーン
	case PlayerSceneState::PLAYER_SELECTSCENE:
		// 選択中のアニメーション

		// 選択後のアニメーション

		break;

	// ゲームシーン
	case PlayerSceneState::PLAYER_GAMESCENE:
		mAnimTypes[static_cast<unsigned int>(PlayerState::PLAYER_STATE_IDLE)] = RENDERER->GetAnimation("Assets/Character/Player1/Animation/Player1_Idle.gpanim", true);
		mAnimTypes[static_cast<unsigned int>(PlayerState::PLAYER_STATE_FORWARD)] = RENDERER->GetAnimation("Assets/Character/Player1/Animation/Player1_Forward.gpanim", true);
		// 残り3方向分のアニメーション

		// 4方向分の走りアニメーション
		mAnimTypes[static_cast<unsigned int>(PlayerState::PLAYER_STATE_RUN_FORWARD)] = RENDERER->GetAnimation("Assets/Character/Player1/Animation/Player1_Sprint_Forward.gpanim", true);

		mAnimTypes[static_cast<unsigned int>(PlayerState::PLAYER_STATE_DIE)] = RENDERER->GetAnimation("Assets/Character/Player1/Animation/Player1_Death.gpanim", false);
		mPlayerBehavior->RegisterState(new PlayerStateIdle(mPlayerBehavior));
		mPlayerBehavior->RegisterState(new PlayerStateForward(mPlayerBehavior));
		mPlayerBehavior->RegisterState(new PlayerStateRunForward(mPlayerBehavior));
		mPlayerBehavior->RegisterState(new PlayerStateDie(mPlayerBehavior));
		mPlayerBehavior->SetFirstState(PlayerStateEnum::Idle);
		break;

	// リザルトシーン
	case PlayerSceneState::PLAYER_RESULT:
		// 勝利アニメーション
		mAnimTypes[static_cast<unsigned int>(PlayerState::PLAYER_STATE_VICTORY)] = RENDERER->GetAnimation("Assets/Character/Player1/Animation/Player1_Victory.gpanim", false);
		mPlayerBehavior->RegisterState(new PlayerStateVictory(mPlayerBehavior));
		mPlayerBehavior->SetFirstState(PlayerStateEnum::Victory);
		break;
	}
}

// ふるまいの登録
void Player1::BehaviorResister()
{
}

// 自身のコライダーのセット
void Player1::SetCollider()
{
	// あたり判定セット
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
	// 当たったオブジェクトのタグ取得
	Tag colliderTag = other->GetTag();

	// 弾がヒットしたら

	const bool isHitEnemyBullet = colliderTag == Tag::ENEMY_BULLET;
	const bool isDash = mPlayerBehavior->GetPlayerState() == PlayerStateEnum::RunForward;

	// 敵の弾に当たったらプレイヤーの体力を１減らす
	if (isHitEnemyBullet &&!mInvincibleComp->GetIsInvicible())
	{
		if (mHitPoint > 0)
		{
			mHitPoint--;
		}

		mInvincibleComp->SetIsInvicible(true);
	}

	// 衝突したのが背景の場合
	if (colliderTag == Tag::BACK_GROUND)
	{
		if (other->GetColliderType() == ColliderTypeEnum::Wall)
		{
			CollisionInfo info = mHitBox->GetCollisionInfo();
			mPosition += info.mFixVec;

			// 位置が変わったのでボックス再計算
			ComputeWorldTransform();
		}
	}

	// 衝突した物体のタグが敵の場合
	if (colliderTag == Tag::ENEMY)
	{
		// 別のタイプがボックスの場合
		if (other->GetColliderType() == ColliderTypeEnum::Box)
		{
			Vector3 fix;

			// 壁とぶつかったとき
			AABB playerBox = mHitBox->GetWorldBox();
			AABB enemyBox = dynamic_cast<BoxCollider*>(other)->GetWorldBox();

			// めり込みを修正
			calcCollisionFixVec(playerBox, enemyBox, fix);

			// 補正ベクトル分戻す
			mPosition = Vector3::Lerp(mPosition, mPosition + fix, 0.1f);
			mPosition.z = 500.0f;

			// 位置が変わったのでボックス再計算
			ComputeWorldTransform();
		}
	}

	// 経験値アイテムに衝突したら経験値追加
	if (colliderTag == Tag::ITEM_EXPERIENCE)
	{
		mExperience++;
	}

	// 回復アイテムに衝突したら体力回復
	if (colliderTag == Tag::ITEM_HEAL)
	{
		mHitPoint += 2;
	}
}

//// プレイヤーに武器をアタッチ
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
