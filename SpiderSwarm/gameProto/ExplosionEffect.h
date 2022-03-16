#pragma once

#include "EffectBase.h"

class ExplosionEffect:public EffectBase
{
public:
	ExplosionEffect();
	virtual ~ExplosionEffect();

	void LoadEffect()override;
	void CreateEffect()override;

	void SetRelativePos(float scale);
	void SetRelativeScale(const float& scale);
private:
	EffectComponent* mEffectComponent;
};

