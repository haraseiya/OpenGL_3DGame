#include "EnemyCharge.h"
#include "ExplosionEffect.h"
#include "EnemyBullet.h"

const float EnemyCharge::mWeakEnemyShotInterval = 1.5f;
const float EnemyCharge::mStrongEnemyShotInterval = 1.5f;
const float EnemyCharge::mBossEnemyShotInterval = 0.5f;
const float EnemyCharge::mChargeSpeed = 0.005f;

EnemyCharge::EnemyCharge(EnemyBehaviorComponent* owner)
	: EnemyState(owner)
{
	mStateType = EnemyStateEnum::Attack2;

	// 突進ターゲットの種類を4か所設置
	mTargetPositions.emplace_back(Vector3(2000,2000,750));
	mTargetPositions.emplace_back(Vector3(-2000, 2000, 750));
	mTargetPositions.emplace_back(Vector3(-2000, -2000, 750));
	mTargetPositions.emplace_back(Vector3(2000, -2000, 750));
}

EnemyCharge::~EnemyCharge()
{
}

EnemyStateEnum EnemyCharge::Update(float deltaTime)
{
	mShootTimer += deltaTime;
	
	// 体力が0以下なら死亡状態へ遷移
	const bool isDead = mOwner->GetHitPoint() <= 0;
	if (isDead)
	{
		return EnemyStateEnum::Death;
	}

	// アニメーションが終了したら待機状態へ遷移
	if (!mOwner->IsAnimationPlaying())
	{
		return EnemyStateEnum::Idle;
	}

	// 自身の種類を判別し弾幕生成
	mEnemyKind = mOwner->GetEnemyKind();
	CreateBarrage(mEnemyKind);


	// 自身の位置と次に向かう位置を取得
	Vector3 enemyPos = mOwner->GetPosition();
	enemyPos.z = 750.0f;
	Vector3 direction = mTargetPos - enemyPos;
	direction.Normalize();
	Vector3 lerpDirection = Vector3::Lerp(mOwner->GetForward(), direction, 1.0f);

	// 次に向かう位置に自分を移動させる
	mOwner->SetPosition(Vector3::Lerp(enemyPos, mTargetPos, mChargeSpeed));
	mOwner->RotateToNewForward(lerpDirection);

	// 攻撃状態を返す
	return EnemyStateEnum::Attack2;
}

// 突進アニメーション開始時
void EnemyCharge::OnEnter()
{
	// 突進ターゲットをランダムで決める
	mTargetPosNum = Math::GetRandom(0, 3);
	mTargetPos = mTargetPositions[mTargetPosNum];

	// 突進アニメーション再生
	mOwner->PlayAnimation(EnemyStateEnum::Attack2);
}

// 突進アニメーション終了時
void EnemyCharge::OnExit()
{
}

// 敵種類に応じた弾幕生成
void EnemyCharge::CreateBarrage(EnemyKind enemyKind)
{
	// 雑魚適用弾幕生成パターン
	if (enemyKind==EnemyKind::ENEMY_WEAK)
	{
		const bool isShot = mShootTimer > mWeakEnemyShotInterval;
		if (isShot)
		{
			mShootTimer = 0.0f;

			// 弾生成
			mEnemyBullet = new EnemyBullet(mOwner, mOwner->GetForward(), 1.0f, 200.0f);
			mEnemyBullet = new EnemyBullet(mOwner, mOwner->GetBack(), 1.0f, 200.0f);
			mEnemyBullet = new EnemyBullet(mOwner, mOwner->GetRight(), 1.0f, 200.0f);
			mEnemyBullet = new EnemyBullet(mOwner, mOwner->GetLeft(), 1.0f, 200.0f);
		}
	}

	// 強敵用弾幕生成パターン
	if (enemyKind == EnemyKind::ENEMY_STRONG)
	{
		// 5秒おきにプレイヤーに向かって弾を発射
		const bool isShot = mShootTimer > mStrongEnemyShotInterval;
		if (isShot)
		{
			mShootTimer = 0.0f;

			Vector3 upperRight = Vector3::UnitX + Vector3::UnitY;		// 右上
			Vector3 upperLeft = Vector3::UnitX + Vector3::NegUnitY;		// 左上
			Vector3 lowerRight = Vector3::NegUnitX + Vector3::UnitY;	// 右下
			Vector3 lowerLeft = Vector3::NegUnitX + Vector3::NegUnitY;	// 左下
			upperRight.Normalize();
			upperLeft.Normalize();
			lowerRight.Normalize();
			lowerLeft.Normalize();

			// 敵弾のインスタンス生成
			mEnemyBullet = new EnemyBullet(mOwner, mOwner->GetForward(), 2.0f, 300.0f);
			mEnemyBullet = new EnemyBullet(mOwner, mOwner->GetBack(), 2.0f, 300.0f);
			mEnemyBullet = new EnemyBullet(mOwner, mOwner->GetRight(), 2.0f, 300.0f);
			mEnemyBullet = new EnemyBullet(mOwner, mOwner->GetLeft(), 2.0f, 300.0f);
			mEnemyBullet = new EnemyBullet(mOwner, mOwner->GetDirectionFromForward(upperRight), 2.0f, 300.0f);
			mEnemyBullet = new EnemyBullet(mOwner, mOwner->GetDirectionFromForward(upperLeft), 2.0f, 300.0f);
			mEnemyBullet = new EnemyBullet(mOwner, mOwner->GetDirectionFromForward(lowerRight), 2.0f, 300.0f);
			mEnemyBullet = new EnemyBullet(mOwner, mOwner->GetDirectionFromForward(lowerLeft), 2.0f, 300.0f);
		}
	}

	// ボス敵弾幕生成パターン
	if (enemyKind == EnemyKind::ENEMY_BOSS)
	{
		// 5秒おきにプレイヤーに向かって弾を発射
		const bool isShot = mShootTimer > mBossEnemyShotInterval;
		if (isShot)
		{
			mShootTimer = 0.0f;

			// 弾の方向決定
			Vector3 upperRight = Vector3::UnitX + Vector3::UnitY;		// 右上
			Vector3 upperLeft = Vector3::UnitX + Vector3::NegUnitY;		// 左上
			Vector3 lowerRight = Vector3::NegUnitX + Vector3::UnitY;	// 右下
			Vector3 lowerLeft = Vector3::NegUnitX + Vector3::NegUnitY;	// 左下
			upperRight.Normalize();
			upperLeft.Normalize();
			lowerRight.Normalize();
			lowerLeft.Normalize();

			// 敵弾のインスタンス生成
			mEnemyBullet = new EnemyBullet(mOwner, mOwner->GetForward(), 2.0f, 300.0f);
			mEnemyBullet = new EnemyBullet(mOwner, mOwner->GetBack(), 2.0f, 300.0f);
			mEnemyBullet = new EnemyBullet(mOwner, mOwner->GetRight(), 2.0f, 300.0f);
			mEnemyBullet = new EnemyBullet(mOwner, mOwner->GetLeft(), 2.0f, 300.0f);
			mEnemyBullet = new EnemyBullet(mOwner, mOwner->GetDirectionFromForward(upperRight), 2.0f, 300.0f);
			mEnemyBullet = new EnemyBullet(mOwner, mOwner->GetDirectionFromForward(upperLeft), 2.0f, 300.0f);
			mEnemyBullet = new EnemyBullet(mOwner, mOwner->GetDirectionFromForward(lowerRight), 2.0f, 300.0f);
			mEnemyBullet = new EnemyBullet(mOwner, mOwner->GetDirectionFromForward(lowerLeft), 2.0f, 300.0f);
		}
	}
}

