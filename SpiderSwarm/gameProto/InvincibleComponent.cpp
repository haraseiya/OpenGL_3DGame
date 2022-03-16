#include "InvincibleComponent.h"
#include "PlayerBase.h"

const int InvincibleComponent::mVisibleValue = 0;			// ���l
const int InvincibleComponent::mInVisibleValue = 5;			// ����l
const float InvincibleComponent::mInvincibleTime = 5.0f;	// ���G����

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

// �X�V����
void InvincibleComponent::Update(float deltaTime)
{
	// ���G��Ԃ̎�
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

	// ���G�^�C�}�[�����G���Ԃ𒴂�����
	if (mInvincibleTimer > mInvincibleTime)
	{
		// �S�Ă̒l��������
		mInvincibleTimer = 0.0f;
		mVisibleCount = 0;
		mOwner->SetVisivle(true);
		mIsInvincible = false;
	}
}
