#pragma once

#include "GameObject.h"
#include <string.h>

class EffectComponent;

class HealEffect : public GameObject
{
public:
	HealEffect();
	~HealEffect();

private:
	EffectComponent* ec;
};

