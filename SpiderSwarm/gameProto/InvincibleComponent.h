#pragma once
#include "Component.h"

class PlayerBase;

class InvincibleComponent : public Component
{
public:
	InvincibleComponent(PlayerBase* owner);
	~InvincibleComponent();

	void Update(float deltaTime) override;

	const bool GetIsInvicible()const { return mIsInvincible; }
	void SetIsInvicible(bool isInvicible) { mIsInvincible = isInvicible; }

private:
	PlayerBase* mOwner;

	static const int mVisibleValue;		// Œ©‚¦‚é‚Æ‚«‚Ì”’l
	static const int mInVisibleValue;	// Œ©‚¦‚È‚¢‚Æ‚«‚Ì”’l

	static const float mInvincibleTime;

	bool mIsInvincible;

	int mVisibleCount;
	float mInvincibleTimer;
};

