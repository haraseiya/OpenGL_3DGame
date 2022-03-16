#include "EnemyChase.h"
#include "PlayerBase.h"
#include "NPCActorBase.h"
#include "GameScene.h"
#include "EnemyBullet.h"

const float EnemyChase::mRange = 10000.0f;
const float EnemyChase::mInterval = 2.0f;

EnemyChase::EnemyChase(EnemyBehaviorComponent* owner,GameObject* target)
	: EnemyState(owner)
	, mTarget(target)
	, mTime(0.0f)
{
	// 敵のアニメーション状態を走りに設定
	mStateType = EnemyStateEnum::Run;
	mAttackInterval = Math::GetRandom(5.0f, 15.0f);
}

EnemyChase::~EnemyChase()
{
}

EnemyStateEnum EnemyChase::Update(float deltaTime)
{
	mDeltaTime = deltaTime;

	// 体力が0以下の場合死亡アニメーションへ遷移
	if (mOwner->GetHitPoint() <= 0)
	{
		return EnemyStateEnum::Death;
	}

	// 5秒たったら突進状態へ移行
	mTime += deltaTime;
	if (mTime >= mAttackInterval)
	{
		mTime = 0.0f;
		//mAttackInterval = Math::GetRandom(5.0f, 15.0f);
		return EnemyStateEnum::Attack2;
	}

	// ターゲットが存在していなければIdle状態に移行
	//const bool isIdle = !mTarget /*|| Math::Abs(mTarget->GetPosition().LengthSq() - mOwnerActor->GetPosition().LengthSq()) > mRange*/;
	//if (!mTarget)
	//{
	//	return EnemyStateEnum::Idle;
	//}

	// キャラクターの前方を取得
	Vector3 charaForwardVec = mOwner->GetForward();

	// プレイヤーへの向きを求める
	Vector3 enemyPos = mOwner->GetPosition();
	enemyPos.z = 750.0f;
	Vector3 playerPos = mTarget->GetPosition();
	Vector3 direction = playerPos- enemyPos;
	direction.Normalize();

	// ある程度離れている場合
	if (direction.LengthSq() > 0.5f)
	{
		// プレイヤーの方向へ向かう
		enemyPos += direction * 100.0f * deltaTime;
		mOwner->SetPosition(enemyPos);

		// 方向キー入力
		charaForwardVec = Vector3::Lerp(mOwner->GetForward(), direction, 0.1f);

		// 進行方向に向けて回転
		charaForwardVec.Normalize();

		mOwner->RotateToNewForward(charaForwardVec);
	}

	// 各種敵ごとのふるまい
	WeakEnemyMove();
	StrongEnemyMove();
	BossEnemyMove();

	// アニメーション続行
	return EnemyStateEnum::Run;
}

void EnemyChase::OnEnter()
{
	// 走りアニメーション再生
	mOwner->PlayAnimation(EnemyStateEnum::Run);
}

void EnemyChase::OnExit()
{
}

void EnemyChase::WeakEnemyMove()
{
	// 5秒おきにプレイヤーに向かって弾発射
	if (mOwner->GetEnemyKind() == EnemyKind::ENEMY_WEAK)
	{
		mShootTimer += mDeltaTime;
		const bool isShot = mShootTimer > mInterval;
		if (isShot)
		{
			mShootTimer = 0.0f;

			// 弾生成
			mEnemyBullet = new EnemyBullet(mOwner, mOwner->GetForward(), 1.0f, 200.0f);
		}
	}
}

void EnemyChase::StrongEnemyMove()
{
	if (mOwner->GetEnemyKind() == EnemyKind::ENEMY_STRONG)
	{
		// 5秒おきにプレイヤーに向かって弾を発射
		mShootTimer += mDeltaTime;
		const bool isShot = mShootTimer > mInterval;
		if (isShot)
		{
			mShootTimer = 0.0f;

			Vector3 upperRight = Vector3::UnitX + Vector3(0.0f, 0.6f, 0.0f);			// 右上
			Vector3 upperLeft = Vector3::UnitX + Vector3(0.0f, -0.6f, 0.0f);		// 左上
			upperRight.Normalize();
			upperLeft.Normalize();

			// 敵弾のインスタンス生成
			mEnemyBullet = new EnemyBullet(mOwner, mOwner->GetForward(), 2.0f, 300.0f);
			mEnemyBullet = new EnemyBullet(mOwner, mOwner->GetDirectionFromForward(upperRight), 2.0f, 300.0f);
			mEnemyBullet = new EnemyBullet(mOwner, mOwner->GetDirectionFromForward(upperLeft), 2.0f, 300.0f);
		}
	}
}

void EnemyChase::BossEnemyMove()
{
	if (mOwner->GetEnemyKind() == EnemyKind::ENEMY_BOSS)
	{
		// 5秒おきにプレイヤーに向かって弾を発射
		mShootTimer += mDeltaTime;
		const bool isShot = mShootTimer > mInterval;
		if (isShot)
		{
			mShootTimer = 0.0f;

			Vector3 upperRight1 = Vector3::UnitX + Vector3(0.0f, 0.3f, 0.0f);			// 右上
			Vector3 upperLeft1 = Vector3::UnitX + Vector3(0.0f, -0.3f, 0.0f);			// 左上
			Vector3 upperRight2 = Vector3::UnitX + Vector3(0.0f, 0.6f, 0.0f);			// 右上
			Vector3 upperLeft2 = Vector3::UnitX + Vector3(0.0f, -0.6f, 0.0f);			// 左上
			upperRight1.Normalize();
			upperLeft1.Normalize();
			upperRight2.Normalize();
			upperLeft2.Normalize();

			// 敵弾のインスタンス生成
			mEnemyBullet = new EnemyBullet(mOwner, mOwner->GetForward(), 3.0f, 200.0f);
			mEnemyBullet = new EnemyBullet(mOwner, mOwner->GetDirectionFromForward(upperRight1), 3.0f, 200.0f);
			mEnemyBullet = new EnemyBullet(mOwner, mOwner->GetDirectionFromForward(upperLeft1), 3.0f, 200.0f);
			mEnemyBullet = new EnemyBullet(mOwner, mOwner->GetDirectionFromForward(upperRight2), 3.0f, 200.0f);
			mEnemyBullet = new EnemyBullet(mOwner, mOwner->GetDirectionFromForward(upperLeft2), 3.0f, 200.0f);
		}
	}
}
