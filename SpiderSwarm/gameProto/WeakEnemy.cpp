#include "WeakEnemy.h"
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
#include "ExplosionEffect.h"
#include "InstanceMeshComponent.h"
#include "EnemyBullet.h"

// 雑魚敵ステート関連
#include "EnemyBehaviorComponent.h"
#include "EnemyIdle.h"
#include "EnemyChase.h"
#include "EnemyAttack.h"
#include "EnemySpawn.h"
#include "EnemyDeath.h"
#include "EnemyRoar.h"
#include "EnemyCharge.h"

#include <iostream>

const float WeakEnemy::mInterval = 2.0f;

WeakEnemy::WeakEnemy(GameObject* target,const Vector3& startPos)
	: mShootTimer(0.0f)
	, mTarget(target)
	, mBullet(nullptr)
{
	// パラメーター初期化
	mPosition = startPos;
	mEnemyKind = EnemyKind::ENEMY_WEAK;
	mEnemyStateScene = EnemyStateScene::ENEMY_SCENE_TITLE;
	mScale = 0.5f; 
	mHitPoint = 5;
	mWalkSpeed = 500.0f;
	mRunSpeed = 500.0f;
	mTurnSpeed = Math::Pi;


	// モデル読み込み
	LoadModel();

	// スケルトン読み込み
	LoadSkeleton();

	// アニメーション読み込み
	LoadAnimation();

	// EnemyBehaviorにふるまいを登録
	BehaviorResister();

	SetCollider();
}

WeakEnemy::~WeakEnemy()
{
	std::cout << "雑魚敵破棄" << std::endl;
}

void WeakEnemy::UpdateActor(float deltaTime)
{
	mTimer += deltaTime;

	// 雑魚敵基本色
	mSkelMeshComponent->SetHitColor(Color::Black);
}

// 当たり判定
void WeakEnemy::OnCollisionEnter(ColliderComponent* own,ColliderComponent* other)
{
	// 衝突したオブジェクトのタグを取得
	Tag otherColliderTag = other->GetTag();

	// プレイヤー弾と衝突したら
	if (otherColliderTag == Tag::PLAYER_BULLET)
	{
		// 被弾色セット
		mSkelMeshComponent->SetHitColor(Color::Red);
		mHitPoint--;
	}

	// 背景と衝突した場合
	if (otherColliderTag == Tag::BACK_GROUND)
	{
		// 修正分の位置を格納
		Vector3 fix;

		//壁とぶつかったとき
		AABB enemyBox = mBoxCollider->GetWorldBox();
		AABB bgBox = dynamic_cast<BoxCollider*>(other)->GetWorldBox();

		// めり込みを修正
		calcCollisionFixVec(enemyBox, bgBox, fix);
	}

	// 敵かプレイヤーに衝突した場合
	if (otherColliderTag == Tag::ENEMY || otherColliderTag == Tag::PLAYER)
	{
		// 修正分の位置が入る
		Vector3 fix;

		//壁とぶつかったとき
		AABB otherEnemyBox = dynamic_cast<BoxCollider*>(other)->GetWorldBox();
		AABB enemyBox = mHitBox->GetWorldBox();

		// めり込みを修正
		calcCollisionFixVec(enemyBox, otherEnemyBox, fix);

		// ベクトルを補正しながら戻す
		mPosition = Vector3::Lerp(mPosition, mPosition + fix, 0.01f);
		mPosition.z = 750.0f;

		// 位置再計算
		ComputeWorldTransform();
	}


	// プレイヤースペシャルショットと衝突時
	if (otherColliderTag == Tag::PLAYER_SPECIAL_SHOT)
	{
		mHitPoint -= 10;
	}
}

void WeakEnemy::FixCollision(BoxCollider* enemy, BoxCollider* player)
{
}

void WeakEnemy::SetAttackHitBox(float scale)
{
	//// 攻撃判定用ボックスの生成
	//mAttackBox = new BoxCollider(this);

	//// 敵前方方向の当たり判定
	//AABB box = mEnemyBox;
	//box.mMin *= 1.5;
	//box.mMax *= 1.5;
	//mAttackBox->SetObjectBox(box);
}

//EnemyBase* WeakEnemy::Clone()
//{
//	//return new WeakEnemy(mTarget);
//	return nullptr;
//}

//void WeakEnemy::RemoveAttackHitBox()
//{
//	if (mAttackBox)
//	{
//		delete mAttackBox;
//		mAttackBox = nullptr;
//	}
//}

//void WeakEnemy::RemoveHitBox()
//{
//	if (mHitBox)
//	{
//		delete mHitBox;
//		mHitBox = nullptr;
//	}
//}

// モデルの読み込み
void WeakEnemy::LoadModel()
{
	mMesh = RENDERER->GetMesh("Assets/Character/Enemy/WeakEnemy/WeakSpider.gpmesh");
	Mesh* mesh = RENDERER->GetMesh("Assets/Character/Enemy/WeakEnemy/EnemyMark.gpmesh");
}

// ボーンの読み込み
void WeakEnemy::LoadSkeleton()
{
	mSkelMeshComponent = new SkeletalMeshComponent(this);
	mSkelMeshComponent->SetMesh(mMesh);
	mSkelMeshComponent->SetSkeleton(RENDERER->GetSkeleton("Assets/Character/Enemy/WeakEnemy/WeakSpider.gpskel"));
}

// アニメーションの読み込み
void WeakEnemy::LoadAnimation()
{
	mEnemyBehaviorComponent = new EnemyBehaviorComponent(this);

	// シーンで使用するアニメーション読み込み
	switch (mEnemyStateScene)
	{
	// タイトルシーン
	case EnemyStateScene::ENEMY_SCENE_TITLE:
		mAnimations.emplace(EnemyStateEnum::Roar, RENDERER->GetAnimation("Assets/Character/Enemy/Animation/Spider_Roar.gpanim", false));
		mAnimations.emplace(EnemyStateEnum::Idle, RENDERER->GetAnimation("Assets/Character/Enemy/Animation/Spider_Idle.gpanim", false));
		mEnemyBehaviorComponent->RegisterState(new EnemyRoar(mEnemyBehaviorComponent));
		mEnemyBehaviorComponent->RegisterState(new EnemyIdle(mEnemyBehaviorComponent));
		mEnemyBehaviorComponent->SetFirstState(EnemyStateEnum::Roar);
		break;

	// ゲームシーン
	case EnemyStateScene::ENEMY_SCENE_GAME:
		mAnimations.emplace(EnemyStateEnum::Spawn, RENDERER->GetAnimation("Assets/Character/Enemy/BossEnemy/BossSpider_Spawn.gpanim", false));
		//mAnimations.emplace(EnemyStateEnum::Idle, RENDERER->GetAnimation("Assets/Character/Enemy/Animation/Spider_Idle.gpanim", false));
		mAnimations.emplace(EnemyStateEnum::Run, RENDERER->GetAnimation("Assets/Character/Enemy/Animation/Spider_Walk.gpanim", true));
		mAnimations.emplace(EnemyStateEnum::Attack2, RENDERER->GetAnimation("Assets/Character/Enemy/Animation/Spider_Charge.gpanim", false));
		mAnimations.emplace(EnemyStateEnum::Death, RENDERER->GetAnimation("Assets/Character/Enemy/Animation/Spider_Death.gpanim", false));
		mEnemyBehaviorComponent->RegisterState(new EnemySpawn(mEnemyBehaviorComponent));
		mEnemyBehaviorComponent->RegisterState(new EnemyIdle(mEnemyBehaviorComponent));
		mEnemyBehaviorComponent->RegisterState(new EnemyChase(mEnemyBehaviorComponent, mTarget));
		mEnemyBehaviorComponent->RegisterState(new EnemyCharge(mEnemyBehaviorComponent));
		mEnemyBehaviorComponent->RegisterState(new EnemyDeath(mEnemyBehaviorComponent));
		mEnemyBehaviorComponent->SetFirstState(EnemyStateEnum::Spawn);
		break;
	}
}

void WeakEnemy::BehaviorResister()
{


}

// 当たり判定をセット
void WeakEnemy::SetCollider()
{
	if (mHitBox) return;
	mEnemyBox = mMesh->GetCollisionBox();
	mEnemyBox.mMin.y *= 0.5f;
	mEnemyBox.mMax.y *= 0.5f;
	mHitBox = new BoxCollider(this);
	mHitBox->SetObjectBox(mEnemyBox);
	mHitBox->SetArrowRotate(true);
}

// 当たり判定を削除
void WeakEnemy::RemoveCollider()
{
	if (mHitBox)
	{
		delete mHitBox;
		mHitBox = nullptr;
	}
}


