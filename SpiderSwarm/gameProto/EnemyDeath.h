#pragma once
#include "EnemyState.h"

class ExplosionEffect;
class ScoreManager;
class ExperienceItem;
class HealItem;

class EnemyDeath:public EnemyState
{
public:
	EnemyDeath(EnemyBehaviorComponent* owner);
	~EnemyDeath();

	EnemyStateEnum Update(float deltaTime) override;

	void           OnEnter() override;
	void           OnExit()  override;

	int GetScore() { return mSumScore; }

private:
	ScoreManager* mScoreManager; 
	ExplosionEffect* mExplosion;
	ExperienceItem* mExperienceItem;
	HealItem* mHealItem;
	int mScore;
	int mSumScore;
};

