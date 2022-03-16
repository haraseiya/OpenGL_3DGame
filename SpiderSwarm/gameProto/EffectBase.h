#pragma once
#include "GameObject.h"

class EffectComponent;

class EffectBase : public GameObject
{
public:
	EffectBase(Tag tag);
	virtual ~EffectBase();

	// エフェクトのアップデート
	void UpdateActor(float deltaTime) override;

	// エフェクトのロード
	virtual void LoadEffect() = 0;

	// エフェクトの生成
	virtual void CreateEffect() = 0;

protected:
	EffectComponent* mEffectComponent;
	float m_lifeCount;
};

