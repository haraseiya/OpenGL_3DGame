#include "EnemyDeath.h"
#include "ExplosionEffect.h"
#include "ScoreManager.h"
#include "ExperienceItem.h"
#include "HealItem.h"

EnemyDeath::EnemyDeath(EnemyBehaviorComponent* owner)
	: EnemyState(owner)
	, mExplosion(nullptr)
	, mExperienceItem(nullptr)
{
	mStateType = EnemyStateEnum::Death;
}

EnemyDeath::~EnemyDeath()
{
	delete mExplosion;
}

EnemyStateEnum EnemyDeath::Update(float deltaTime)
{
	// 死亡アニメーションが終了したら
	if (!mOwner->IsAnimationPlaying())
	{
		return EnemyStateEnum::Spawn;
	}

	return EnemyStateEnum::Death;
}


void EnemyDeath::OnEnter()
{
	// 死亡アニメーション再生
	mOwner->PlayAnimation(EnemyStateEnum::Death);


	//mOwner->RemoveCollider();
}

// 死亡アニメーション終了時の処理
void EnemyDeath::OnExit()
{
	int score = 0;

	// 自身を未使用状態へ移行
	mOwner->SetIsActive(false);

	mExplosion = new ExplosionEffect();

	if (mOwner->GetEnemyKind() == EnemyKind::ENEMY_WEAK)
	{
		mExperienceItem = new ExperienceItem(mOwner->GetPosition());
		mExplosion->SetScale(1.0f);
		mExplosion->LoadEffect();
		mExplosion->CreateEffect();
		mExplosion->SetPosition(mOwner->GetPosition());
		mExplosion->SetRelativePos(100.0f);

		ScoreManager::GetInstance()->AddScore(1000);
		mSumScore += 1000;
	}
	if (mOwner->GetEnemyKind() == EnemyKind::ENEMY_STRONG)
	{
		mHealItem = new HealItem(mOwner->GetPosition());

		mExplosion->SetScale(3.0f);
		mExplosion->LoadEffect();
		mExplosion->CreateEffect();
		mExplosion->SetPosition(mOwner->GetPosition());
		mExplosion->SetRelativePos(200.0f);

		ScoreManager::GetInstance()->AddScore(5000);
		mSumScore += 5000;
	}
	if (mOwner->GetEnemyKind() == EnemyKind::ENEMY_BOSS)
	{
		mExplosion->LoadEffect();
		mExplosion->CreateEffect();
		mExplosion->SetPosition(mOwner->GetPosition());
		mExplosion->SetRelativePos(300.0f);

		ScoreManager::GetInstance()->AddScore(20000);
		mSumScore += 20000;
		mOwner->SetIsBossDeadFlag(true);
	}
	mOwner->SetPosition(Vector3(5000, 5000, 5000));
	mOwner->SetState(GameObject::State::STATE_PAUSED);
}
