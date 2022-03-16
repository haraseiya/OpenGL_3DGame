#pragma once

#include "EffectBase.h"
#include <string>

class EffectComponent;

class ChantEffect :public EffectBase
{
public:
	ChantEffect(GameObject* owner,bool loop);
	virtual ~ChantEffect();

	void UpdateActor(float deltaTime) override;

	float GetLifeCount() { return m_lifeCount; }

private:
	float m_lifeCount;
};