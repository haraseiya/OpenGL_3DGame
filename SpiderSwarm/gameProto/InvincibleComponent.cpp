#include "InvincibleComponent.h"
#include "PlayerBase.h"

const int InvincibleComponent::mVisibleValue = 0;			// 可視値
const int InvincibleComponent::mInVisibleValue = 5;			// 非可視値
const float InvincibleComponent::mInvincibleTime = 5.0f;	// 無敵時間

InvincibleComponent::InvincibleComponent(PlayerBase* owner)
	: Component(owner)
	, mOwner(owner)
	, mIsInvincible(false)
	, mVisibleCount(0)
{
}

InvincibleComponent::~InvincibleComponent()
{
}

// 更新処理
void InvincibleComponent::Update(float deltaTime)
{
	// 無敵状態の時
	if (mIsInvincible)
	{
		mInvincibleTimer += deltaTime;
		mVisibleCount++;

		const int divisionNum = 10;

		if (mVisibleCount % divisionNum == mVisibleValue)
		{
			mOwner->SetVisivle(true);
		}
		else if (mVisibleCount % divisionNum == mInVisibleValue)
		{
			mOwner->SetVisivle(false);
		}
	}

	// 無敵タイマーが無敵時間を超えたら
	if (mInvincibleTimer > mInvincibleTime)
	{
		// 全ての値を初期化
		mInvincibleTimer = 0.0f;
		mVisibleCount = 0;
		mOwner->SetVisivle(true);
		mIsInvincible = false;
	}
}
