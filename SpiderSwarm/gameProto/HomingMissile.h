#pragma once
#include "EffectBase.h"

class GameObject;
class EffectComponent;

class HomingMissile : public EffectBase
{
public:
	HomingMissile(GameObject* owner, bool loop);
	~HomingMissile();

private:
	EffectComponent* mEffectComponent;	// �G�t�F�N�g�R���|�[�l���g
};

